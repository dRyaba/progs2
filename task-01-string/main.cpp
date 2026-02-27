#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

class String {
    int len;
    char *string;

    int find_subs(char *string1, const char *substring, size_t len1,
                  size_t sub_len, int i) {
        for (; i < len1; i++)
            if (string1[i] == substring[0]) {
                int j = 0;
                for (; j < sub_len; j++)
                    if (string1[i + j] != substring[j])
                        break;
                if (j == sub_len)
                    return i;
            }
        return -1;
    }

public:
    String(int l, char *s) : len(l), string(s) {}
    String() : len(0), string(nullptr) {}
    String(char b) {
        len = 1;
        string = new char[len + 1];
        string[0] = b;
        string[1] = 0;
    }
    String(const String &inst) {
        len = inst.len;
        string = new char[len + 1];
        strncpy(string, inst.string, len + 1);
    }
    String(const char *string1) {
        string = new char[strlen(string1) + 1];
        len = (int)strlen(string1);
        strncpy(string, string1, len + 1);
    }

    String &operator=(const String &inst) {
        if (this == &inst)
            return *this;
        delete[] string;
        len = inst.len;
        string = new char[len + 1];
        strncpy(string, inst.string, len + 1);
        return *this;
    }

    String operator+=(const String &string1) {
        len = len + string1.len + 1;
        char *temp = string;
        string = new char[len];
        string[0] = 0;
        strcat(string, temp);
        delete[] temp;
        strcat(string, string1.string);
        return *this;
    }

    String operator()(int a, int b) const {
        if (a > b || a < 0 || b > len)
            throw runtime_error("Invalid substring range");
        char *result = new char[b - a + 2];
        for (int i = 0; i <= (b - a); i++)
            result[i] = string[a + i];
        result[b - a + 1] = 0;
        return {b - a + 1, result};
    }

    String operator*(int a) const {
        int n = len * a;
        char *result = new char[n + 1];
        for (int i = 0; i < a; i++)
            for (int j = 0; j < len; j++)
                result[i * len + j] = string[j];
        result[n] = 0;
        return {n, result};
    }

    char &operator[](unsigned int index) const {
        if (index >= (unsigned)len)
            throw runtime_error("Index out of bounds");
        return string[index];
    }

    String operator-(const String &substring) {
        for (int i = 0; i < len - substring.len; i++) {
            int r = find_subs(string, substring.string, len, substring.len, i);
            if (r != -1) {
                for (int k = r; k < len - substring.len; k++)
                    string[k] = string[k + substring.len];
                len -= substring.len;
                string[len] = 0;
            }
        }
        return *this;
    }

    int getLen() const { return len; }
    char *getStr() const { return string; }

    char *my_getline(std::istream &istream) {
        char *str = new char[1];
        str[0] = 0;
        char ch;
        char *buf;
        size_t length;

        while (istream.get(ch) && ch != '\n') {
            length = strlen(str);
            buf = new char[length + 2];
            strcpy(buf, str);
            buf[length] = ch;
            buf[length + 1] = 0;
            delete[] str;
            str = buf;
        }
        return str;
    }

    friend std::istream &operator>>(std::istream &stream, String &instance) {
        if (instance.string)
            delete[] instance.string;
        char *temp = instance.my_getline(stream);
        instance.len = (int)strlen(temp);
        instance.string = new char[instance.len + 1];
        strncpy(instance.string, temp, instance.len + 1);
        delete[] temp;
        return stream;
    }

    friend std::ostream &operator<<(std::ostream &stream, const String &instance) {
        stream << instance.string;
        return stream;
    }

    bool replace(const String &x, const String &y) {
        int count = 0;
        size_t temp;
        for (int i = 0; i < len - x.getLen(); i++) {
            temp = find_subs(string, x.getStr(), len, x.getLen(), i);
            if (temp != (size_t)-1) {
                count++;
                i = (int)(temp + x.getLen());
            }
        }
        if (!count)
            return false;
        int shift = x.getLen() - y.getLen();
        int newLen = len - count * shift + 1;
        char *res1 = new char[newLen];
        int tmp, tmp2 = 0;
        for (int k = 0; k < count; k++) {
            tmp = find_subs(string, x.getStr(), len, x.getLen(), tmp2);
            for (int i = tmp2; i < tmp; i++)
                res1[i - k * shift] = string[i];
            for (int i = tmp; i < tmp + y.getLen(); i++)
                res1[i - k * shift] = y.getStr()[i - tmp];
            tmp2 = tmp + x.getLen();
        }
        for (int i = tmp2; i < len + 1; i++)
            res1[i - count * shift] = string[i];
        delete[] this->string;
        this->string = res1;
        len = newLen - 1;
        return true;
    }

    ~String() { delete[] string; }
};

String operator+(const String &string1, const String &string2) {
    String temp = string1;
    temp += string2;
    return temp;
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    String str1, str2, str3;
    in >> str1 >> str2 >> str3;
    str1.replace(str2, str3);
    out << str1;
    return 0;
}
