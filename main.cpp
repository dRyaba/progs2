#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

using namespace std;

template<typename T>
class Vector {
    int size;
    T *elements;
    int alloc_mem;

    void Copy(const Vector &vector) {
        size = vector.size;
        alloc_mem = vector.alloc_mem;
        elements = new T[size];
        for (int i = 0; i < size; i++)
            elements[i] = vector.elements[i];
    }

public:
    Vector() : size(0), elements(new T[1]), alloc_mem(1) {}

    Vector(const Vector &vector) {
        Copy(vector);
    }

    Vector &operator=(const Vector &vector) {
        if (this != &vector) {
            delete[] elements;
            Copy(vector);
        }
        return *this;
    }

    ~Vector() {
        delete[] elements;
    }

    void add(T element) {
        if (size + 1 > alloc_mem) {
//            alloc_mem > 0 ? alloc_mem *= 2 : alloc_mem = 1;
            alloc_mem *= 2;
            T *temp = new T[alloc_mem];
            for (int i = 0; i < size; i++) {
                temp[i] = elements[i];
            }
            delete[] elements;
            elements = temp;
        }
        elements[size] = element;
        ++size;
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            throw runtime_error("ERROR");
        }
        T *temp = new T[size - 1];
        for (int i = 0; i < index; i++)
            temp[i] = elements[i];
        for (int i = index + 1; i < size; i++)
            temp[i - 1] = elements[i];
        delete[] elements;
        --size;
        elements = new T[size];
//        elements = temp;
        for (int i = 0; i < size; i++)
            elements[i] = temp[i];
        delete[] temp;
    }

    T &operator[](int r) {
        if (r < 0 || r >= size) {
            throw runtime_error("ERROR");
//            cout<<"ERROR";
//            exit(0);
        }
        return elements[r];
    }

    void print(int index) {
        if (index < 0 || index >= size) {
            throw runtime_error("ERROR");
        }
        cout << elements[index] << endl;
    }

//    void update(int index, T value) {
//        if (index < 0 || index >= size) {
//            throw runtime_error("ERROR");
//        }
//        elements[index] = value;
//    }

    void lsh(int r) {
        if (!size) {
            throw runtime_error("ERROR");
        }
        int t = size - (r % size);
        if (!t)
            return;
        T *temp = new T[size];
        for (int i = 0; i < size; i++)
            temp[(i + t) % size] = elements[i];
        delete[] elements;
        elements = new T[size];
        for (int i = 0; i < size; i++)
            elements[i] = temp[i];
        delete[] temp;
    }

    void rsh(int r) {
        if (!size) {
            throw runtime_error("ERROR");
        }
        int t = r % size;
        if (!t)
            return;
        T *temp = new T[size];
        for (int i = 0; i < size; i++)
            temp[(i + t) % size] = elements[i];
        delete[] elements;
        elements = temp;
    }

    void printAll() {
        for (int i = 0; i < size; i++)
            cout << elements[i] << endl;
    }
};

template<typename T>
void execute() {
    Vector<T> v;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string c;
        cin >> c;
        T value;
        int index;
        if (c == "ADD") {
            cin >> value;
            v.add(value);
        }
        if (c == "REMOVE") {
            cin >> index;
            try { v.remove(index); }
            catch (...) {
                cout << "ERROR";
                exit(0);
            }
        }
        if (c == "PRINT") {
            cin >> index;
            try { v.print(index); }
            catch (...) {
                cout << "ERROR";
                exit(0);
            }
        }
        if (c == "UPDATE") {
            cin >> index >> value;
            try { v[index] = value; }
            catch (...) {
                cout << "ERROR";
                exit(0);
            }
        }
        int shift;
        if (c == "LSH") {
            cin >> shift;
            try { v.lsh(shift); }
            catch (...) {
                cout << "ERROR";
                exit(0);
            }
        }
        if (c == "RSH") {
            cin >> shift;
            try { v.rsh(shift); }
            catch (...) {
                cout << "ERROR";
                exit(0);
            }
        }
    }
    v.printAll();
}

int main() {
    char t;
    cin >> t;
    if (t == 'I') execute<int>();
    if (t == 'D') {
        cout << setprecision(2) << fixed;
        execute<double>();
    }
    if (t == 'S') execute<string>();
}