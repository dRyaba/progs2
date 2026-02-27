#include <fstream>
#include <map>
#include <queue>
#include <stack>
#include <stdexcept>

using namespace std;

class OperationSet {
    map<char, int> priority;
    map<char, char> associativity;

public:
    void insert(char sign, int p, char s) {
        priority[sign] = p;
        associativity[sign] = s;
    }

    int prior(char sign) { return priority[sign]; }

    bool isLeftAssociative(char sign) {
        return associativity[sign] == 'L';
    }
};

char type(char c) {
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 's';
    if (isdigit(c))
        return 'd';
    if (c == '(')
        return 'l';
    return 'r';
}

bool sortFacility(queue<char> &outQueue, OperationSet &Op, const string &basicString) {
    stack<char> Stack;
    char sc;

    for (char i : basicString) {
        switch (type(i)) {
            case 's': {
                while (!Stack.empty()) {
                    sc = Stack.top();
                    if ((type(sc) == 's') && (((Op.isLeftAssociative(i)) && (Op.prior(i) <= Op.prior(sc))) ||
                                             ((!Op.isLeftAssociative(i)) && (Op.prior(i) < Op.prior(sc))))) {
                        outQueue.push(sc);
                        Stack.pop();
                    } else
                        break;
                }
                Stack.push(i);
                break;
            }
            case 'd':
                outQueue.push(i);
                break;
            case 'l':
                Stack.push(i);
                break;
            case 'r': {
                while (!Stack.empty()) {
                    sc = Stack.top();
                    if (sc == '(')
                        break;
                    outQueue.push(sc);
                    Stack.pop();
                }
                Stack.pop();
                break;
            }
            default:
                return false;
        }
    }

    while (!Stack.empty()) {
        outQueue.push(Stack.top());
        Stack.pop();
    }
    return true;
}

int resolve(queue<char> &pQueue) {
    char cur;
    int res, curr;
    stack<int> workStack;

    while (!pQueue.empty()) {
        cur = pQueue.front();
        switch (type(cur)) {
            case 's': {
                res = workStack.top();
                workStack.pop();
                curr = workStack.top();
                workStack.pop();

                switch (cur) {
                    case '+': res += curr; break;
                    case '-': res = curr - res; break;
                    case '*': res *= curr; break;
                    case '/':
                        if (res == 0)
                            throw runtime_error("division by zero");
                        res = curr / res;
                        break;
                    default: break;
                }
                workStack.push(res);
                break;
            }
            case 'd':
                workStack.push(cur - '0');
                break;
            default:
                throw runtime_error("incorrect type");
        }
        pQueue.pop();
    }

    if (workStack.size() == 1)
        return workStack.top();
    throw runtime_error("stack error");
}

int main() {
    char sign, side;
    int priority;
    queue<char> outQueue;
    OperationSet set;
    ifstream in("input.txt");
    ofstream out("output.txt");
    for (int i = 0; i < 4; i++) {
        in >> sign >> priority >> side;
        set.insert(sign, priority, side);
    }
    string basicString;
    in >> basicString;
    if (sortFacility(outQueue, set, basicString)) {
        try {
            out << resolve(outQueue);
        } catch (...) {
            out << "ERROR";
        }
    }
    return 0;
}
