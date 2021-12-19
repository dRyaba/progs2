#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <set>
#include <ctime>
#include <algorithm>

#define ThreadsNumber 4
using namespace std;

mutex storageMutex;
condition_variable storageMutexCondVar;
string globalOutput = R"(C:\Users\User\CLionProjects\5.1.1\cmake-build-debug\output\)", globalInput;

vector<string> UniqueStorage;
queue<string> URLqueue;
atomic<bool> flag;
int uniq = 0;

void findURL() {
    while (true) {
        unique_lock<mutex> Lock(storageMutex/*, defer_lock*/);
        storageMutexCondVar.wait(Lock, []() -> bool { return !URLqueue.empty() || flag.load(); });

        if (URLqueue.empty())
            break;
        //Lock.lock();
        string URL = URLqueue.front();
        uniq++;
        URLqueue.pop();
        Lock.unlock();

        string content;
        ifstream input(globalInput + URL);
        getline(input, content);
        input.close();

        size_t start = 0;

        while (start != string::npos) {
            start = content.find("<a href=", start);
            if (start != string::npos) {
                start = content.find("://", start);
                size_t len = content.find('>', start) - start - 4;
                URL = content.substr(start + 3, len);
                start += len;

                Lock.lock();
                if (::std::find(UniqueStorage.begin(), UniqueStorage.end(), URL) == UniqueStorage.end()) {
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

//                    cout << URLqueue.size() << " " << URL << endl;
                    Lock.lock();
//                    URLqueue.push(temp);
                    URLqueue.push(URL);
                    Lock.unlock();

                    storageMutexCondVar.notify_all();
                } else Lock.unlock();
            } else break;
        }
    }
}

int main() {
    clock_t start_time = clock();
    ifstream in("input.txt");

    string st_adr, workString;

    in >> globalInput;
    in >> st_adr;
    ifstream fin(globalInput + st_adr);

    getline(fin, workString);
    fin.close();
    ofstream fout(globalOutput + st_adr);
    fout << workString;
    fout.close();
    UniqueStorage.push_back(st_adr);
    flag.store(false);
    vector<thread> Threads;
    Threads.reserve(ThreadsNumber);
    URLqueue.push(st_adr);
    for (int i = 0; i < ThreadsNumber; i++)
        Threads.emplace_back(thread(findURL));
    storageMutexCondVar.notify_one();
    while (!URLqueue.empty())
        this_thread::sleep_for(chrono::milliseconds(1));

    flag.store(true);
    for (int i = 0; i < ThreadsNumber; i++) {
        storageMutexCondVar.notify_one();
    }

    for (int i = 0; i < ThreadsNumber; i++)
        Threads[i].join();

    clock_t end_time = clock();
    cout << "Successfully ended. time spent: " << (end_time - start_time) / 1000.00 << " seconds. uniq: "
         << uniq;
//    to save results of running programm with different amount of threads
//    ofstream resout(R"(C:\Users\User\CLionProjects\5.1.1\cmake-build-debug\Results.txt)", ofstream::app);
//    resout << "Threads: " << ThreadsNumber << " time spent: " << (end_time - start_time) / 1000.00
//           << " seconds. uniq elements: "
//           << uniq << endl;
//    resout.close();
}