#include <iostream>
#include <string>
#include <iomanip>
bool flag = false;

using namespace std;

template<typename T>
class Vector {
    int size;
    T *elements;

    void Copy(const Vector &vector) {
        size = vector.size;
        elements = new T[size];
        for (int i = 0; i < size; i++)
            elements[i] = vector.elements[i];
    }

public:
    Vector() {
        size = 0;
        elements = nullptr;
    }

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
        T *temp = new T[size + 1];
        for (int i = 0; i < size; i++) {
            temp[i] = elements[i];
        }
        temp[size] = element;
        delete[] elements;
        ++size;
        elements = new T[size];
        for (int i = 0; i < size; i++)
            elements[i] = temp[i];
        delete[] temp;
    }

    void remove(int index) {
        if (index < 0 || index >= size) {
            flag = true;
            return;
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
            flag = true;
            return T();
        }
        return elements[r];
    }

    void print(int index) {
        if (index < 0 || index >= size) {
            flag = true;
            return;
        }
        cout << elements[index] << endl;
    }

    void update(int index, T value) {
        if (index < 0 || index >= size) {
            flag = true;
            return;
        }
        elements[index] = value;
    }

    void lsh(int r) {
        if (!size) {
            flag = true;
            return;
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
            flag = true;
            return;
        }
        int t = r % size;
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
            v.remove(index);
        }
        if (c == "PRINT") {
            cin >> index;
            v.print(index);
        }
        if (c == "UPDATE") {
            cin >> index >> value;
            v.update(index, value);
        }
        int shift;
        if (c == "LSH") {
            cin >> shift;
            v.lsh(shift);
        }
        if (c == "RSH") {
            cin >> shift;
            v.rsh(shift);
        }
        if (flag) {
            cout << "ERROR";
            break;
        }
    }
    v.printAll();
}

int main() {
    char t;
    cin >> t;
    if (t == 'I') execute<int>();
    if (t == 'D') {
        cout<<setprecision(2)<<fixed;
        execute<double>();
    }
    if (t == 'S') execute<string>();
}
