#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <algorithm>

using namespace std;

mutex storageMutex;
condition_variable storageMutexCondVar;
string globalOutput = "output/", globalInput;
vector<string> UniqueStorage;
queue<string> URLqueue;
atomic<bool> flag;
int uniq = 0;

void findURL() {
    while (true) {
        unique_lock<mutex> Lock(storageMutex);
        storageMutexCondVar.wait(Lock, []() { return !URLqueue.empty() || flag.load(); });
        if (URLqueue.empty() && flag.load())
            break;
        string URL = URLqueue.front();
        uniq++;
        URLqueue.pop();
        Lock.unlock();

        string content;
        ifstream copyInput(globalInput + URL);
        getline(copyInput, content);
        copyInput.close();
        size_t start = 0;
        while (start != string::npos) {
            start = content.find("<a href=", start);
            if (start != string::npos) {
                start = content.find("://", start) + 3;
                size_t len = content.find('>', start) - start - 1;
                URL = content.substr(start, len);
                start += len;

                Lock.lock();
                if (find(UniqueStorage.begin(), UniqueStorage.end(), URL) == UniqueStorage.end()) {
                    UniqueStorage.push_back(URL);
                    Lock.unlock();

                    ofstream output(globalOutput + URL);
                    ifstream input(globalInput + URL);
                    if (!output.is_open() || !input.is_open())
                        throw runtime_error("OPEN_ERROR");

                    string temp;
                    getline(input, temp);
                    input.close();
                    output << temp;
                    output.close();

                    Lock.lock();
                    URLqueue.push(URL);
                    Lock.unlock();

                    storageMutexCondVar.notify_all();
                } else Lock.unlock();
            } else break;
        }
    }
}

int main() {
    int threadsNumber;
    string st_adr, workString;
    ifstream in("input.txt");
    in >> globalInput >> st_adr >> threadsNumber;
    in.close();

    ifstream fin(globalInput + st_adr);
    getline(fin, workString);
    fin.close();

    ofstream fout(globalOutput + st_adr);
    fout << workString;
    fout.close();

    UniqueStorage.push_back(st_adr);
    flag.store(false);
    vector<thread> Threads;
    Threads.reserve(threadsNumber);
    URLqueue.push(st_adr);

    auto start_time = chrono::steady_clock::now();

    for (int i = 0; i < threadsNumber; i++)
        Threads.emplace_back(findURL);
    storageMutexCondVar.notify_one();

    while (true) {
        unique_lock<mutex> Lock(storageMutex);
        if (!URLqueue.empty()) {
            Lock.unlock();
            break;
        }
        Lock.unlock();
        this_thread::sleep_for(chrono::milliseconds(1));
    }

    flag.store(true);
    for (int i = 0; i < threadsNumber; i++)
        storageMutexCondVar.notify_one();

    for (int i = 0; i < threadsNumber; i++)
        Threads[i].join();

    auto end_time = chrono::steady_clock::now();
    auto elapsed = chrono::duration<double>(end_time - start_time).count();

    cout << uniq << " " << elapsed;
    return 0;
}
