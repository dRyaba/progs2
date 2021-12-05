#include <fstream>
#include <map>
#include <queue>
#include <stack>

using namespace std;

ifstream in("input.txt");
ofstream out("output.txt");

class operationSet {
    map<char, int> priority;
    map<char, char> associativity;

public:
    void insert(char sign, int p, char s) {
        this->priority[sign] = p;
        this->associativity[sign] = s;
    }

    int prior(char sign) {
        return priority[sign];
    }

    int assoc(char sign) {
        return associativity[sign] == 'L' ? 1 : 0;
    }
};

char type(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 's';
    else if (isdigit(c))
        return 'd';
    else if (c == '(')
        return 'l';
//    else if (c == ')')
    return 'r';
}

bool sortFacility(queue<char> *outQueue, operationSet *Op) {
    stack<char> Stack;
    char sc;

    string basicString;
    in >> basicString;

    for (char i: basicString) {

        switch (type(i)) {
            case 's': {
                while (!Stack.empty()) {
                    sc = Stack.top();

                    if ((type(sc) == 's') && (
                            ((Op->assoc(i)) && (Op->prior(i) <= Op->prior(sc))) ||
                            ((!Op->assoc(i)) && (Op->prior(i) < Op->prior(sc))))) {
                        outQueue->push(sc);
                        Stack.pop();
                    } else
                        break;

                }
                Stack.push(i);
                break;
            }
            case 'd': {
                outQueue->push(i);
                break;
            }
            case 'l': {
                Stack.push(i);
                break;
            }
            case 'r': {
                while (!Stack.empty()) {
                    sc = Stack.top();
                    if (sc == '(')
                        break;
                    else {
                        outQueue->push(sc);
                        Stack.pop();
                    }
                }
                Stack.pop();
                break;
            }
            default:
                return false;
        }
    }

    while (!Stack.empty()) {
        outQueue->push(Stack.top());
        Stack.pop();
    }
    return true;
}

int resolve(queue<char> *pQueue) {
    char cur;
    int res, curr;
    stack<int> workStack;

    while (!pQueue->empty()) {
        cur = pQueue->front();
        switch (type(cur)) {
            case 's': {
                res = workStack.top();
                workStack.pop();
                curr = workStack.top();
                workStack.pop();

                switch (cur) {
                    case '+': {
                        res += curr;
                        break;
                    }
                    case '-': {
                        res = curr - res;
                        break;
                    }
                    case '*': {
                        res *= curr;
                        break;
                    }
                    case '/': {
                        if (res == 0)
                            throw -1;
                        res = curr / res;
                        break;
                    }
                    default:
                        break;
                }
                workStack.push(res);
                break;
            }
            case 'd': {
                workStack.push(cur - '0');
                break;
            }
            default:
                throw -1;
        }
        pQueue->pop();
    }

    if (workStack.size() == 1)
        return workStack.top();
    else
        throw -1;
}

int main() {
    char sign, side;
    int priority;
    queue<char> outQueue;
    operationSet set;

    for (int i = 0; i < 4; i++) {
        in >> sign >> priority >> side;
        set.insert(sign, priority, side);
    }
    if (sortFacility(&outQueue, &set)) {
        try {
            out << resolve(&outQueue);
        }
        catch (...) {
            out << "ERROR";
        }
    }
}