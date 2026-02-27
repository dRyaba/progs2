#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>
#include <fstream>

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
    Vector(const Vector &vector) { Copy(vector); }

    int getSize() { return size; }

    Vector &operator=(const Vector &vector) {
        if (this != &vector) {
            delete[] elements;
            Copy(vector);
        }
        return *this;
    }

    ~Vector() { delete[] elements; }

    void add(T element) {
        if (size + 1 > alloc_mem) {
            alloc_mem > 0 ? alloc_mem *= 2 : alloc_mem = 1;
            T *temp = new T[alloc_mem];
            for (int i = 0; i < size; i++)
                temp[i] = elements[i];
            delete[] elements;
            elements = temp;
        }
        elements[size] = element;
        ++size;
    }

    void remove(int index) {
        if (index < 0 || index >= size)
            throw runtime_error("ERROR");
        T *temp = new T[size - 1];
        for (int i = 0; i < index; i++)
            temp[i] = elements[i];
        for (int i = index + 1; i < size; i++)
            temp[i - 1] = elements[i];
        delete[] elements;
        --size;
        elements = temp;
    }

    T &operator[](int r) {
        if (r < 0 || r >= size)
            throw runtime_error("ERROR");
        return elements[r];
    }

    void lsh(int r) {
        if (!size)
            throw runtime_error("ERROR");
        int t = size - (r % size);
        if (!t) return;
        T *temp = new T[size];
        for (int i = 0; i < size; i++)
            temp[(i + t) % size] = elements[i];
        delete[] elements;
        elements = temp;
    }

    void rsh(int r) {
        if (!size)
            throw runtime_error("ERROR");
        int t = r % size;
        if (!t) return;
        T *temp = new T[size];
        for (int i = 0; i < size; i++)
            temp[(i + t) % size] = elements[i];
        delete[] elements;
        elements = temp;
    }

    void printAll(ostream &out) {
        for (int i = 0; i < size; i++)
            out << elements[i] << endl;
    }
};

template<typename T>
void execute(istream &in, ostream &out) {
    Vector<T> v;
    int n;
    in >> n;
    try {
        for (int i = 0; i < n; i++) {
            string c;
            in >> c;
            T value;
            int index;
            if (c == "ADD") {
                in >> value;
                v.add(value);
            } else if (c == "REMOVE") {
                in >> index;
                v.remove(index);
            } else if (c == "PRINT") {
                in >> index;
                out << v[index] << endl;
            } else if (c == "UPDATE") {
                in >> index >> value;
                v[index] = value;
            } else if (c == "LSH") {
                int shift;
                in >> shift;
                v.lsh(shift);
            } else if (c == "RSH") {
                int shift;
                in >> shift;
                v.rsh(shift);
            }
        }
        v.printAll(out);
    } catch (...) {
        out << "ERROR" << endl;
    }
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    char t;
    in >> t;
    if (t == 'I') execute<int>(in, out);
    else if (t == 'D') {
        out << setprecision(2) << fixed;
        execute<double>(in, out);
    }
    else if (t == 'S') execute<string>(in, out);
    return 0;
}
