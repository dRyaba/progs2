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
    Vector() : size(0), elements(new T[1]), alloc_mem(0) {}

    Vector(const Vector &vector) {
        Copy(vector);
    }

    int getSize() {
        return size;
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
            alloc_mem > 0 ? alloc_mem *= 2 : alloc_mem = 1;
//            alloc_mem *= 2;
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
            throw index;
        }
        T *temp = new T[size - 1];
        for (int i = 0; i < index; i++)
            temp[i] = elements[i];
        for (int i = index + 1; i < size; i++)
            temp[i - 1] = elements[i];
        delete[] elements;
        --size;
        elements = temp;
//        for (int i = 0; i < size; i++)
//            elements[i] = temp[i];
//        delete[] temp;
    }

    T &operator[](int r) {
        if (r < 0 || r >= size) {
            throw r;
//            cout<<"ERROR";
//            exit(0);
        }
        return elements[r];
    }

    void print(int index) {
        if (index < 0 || index >= size) {
            throw index;
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
            throw runtime_error("Size - 0: shift can't be made");
        }
        int t = size - (r % size);
        if (!t)
            return;
        T *temp = new T[size];
        for (int i = 0; i < size; i++)
            temp[(i + t) % size] = elements[i];
        delete[] elements;
        elements = temp;
    }

    void rsh(int r) {
        if (!size) {
            throw runtime_error("Size - 0: shift can't be made");
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
    try {
        for (int i = 0; i < n; i++) {
            string c;
            cin >> c;
            T value;
            int index;
            if (c == "ADD") {
                cin >> value;
                v.add(value);
            } else if (c == "REMOVE") {
                cin >> index;
                v.remove(index);
            } else if (c == "PRINT") {
                cin >> index;
                v.print(index);
            } else if (c == "UPDATE") {
                cin >> index >> value;
                v[index] = value;
            } else {
                int shift;
                if (c == "LSH") {
                    cin >> shift;
                    v.lsh(shift);
                } else if (c == "RSH") {
                    cin >> shift;
                    v.rsh(shift);
                }
            }
        }
    }
    catch (int index) {
        cout << "index out of bounce: index - " << index << " min index - 0; max index - " << v.getSize() - 1 << endl;
        return;
    }
    catch (const char *size_error) {
        cout << size_error;
        return;
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
//S
//5
//ADD Ivanov
//ADD Petrova
//ADD Sidorov
//REMOVE 3
//LSH 1