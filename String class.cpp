#include <iostream>
//#include <fstream>
#include <cstring>

using namespace std;

//size_t myStrlen(const char *s) {
//    const char *cur = s;
//    for (; *cur; cur++);
//    return cur - s;//выводим разницу между указателем на первый элемент и последний
//}

//char *copy(char *dst, const char *src) {
//    while ((*(dst++) = *(src++)) != 0);
//    return dst;
//}

//char *myStrcat(char *dst, const char *src) {
//    return copy(dst + myStrlen(dst), src);
//}

int find_subs(char *string, const char *substring, int len,
              long int sub_len,
              int i) { //находит подстроку в строке и возвращает указатель на её начало, поиск идёт с i элемента
    for (i; i < len; i++)
        if (string[i] == substring[0]) {
            int j = 0;
            for (; j < sub_len; j++)
                if (string[i + j] != substring[j])
                    break;
            if (j == sub_len) {
                return i;
            }
        }
    return -1;
}

class String {

    long int len;
    char *string;
public:
    String(int l, char *s) : len(l), string(s) {}

    String() {
        len = 0;
        string = nullptr;
    }

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
        len = strlen(string1);
        strncpy(string, string1, len + 1);
    }

    String &operator=(const String &inst) {
        if (this == &inst)
            throw;
        delete[] string;
        len = inst.len;
        string = new char[len + 1];
        strncpy(string, inst.string, len + 1);
        return *this;
    }

    String& operator+=(const String &string1) {
        len = len + string1.len + 1;
        strcat(string, string1.string);
        return *this;
    }

    String operator()(int a, int b) {
        if (a > b || a < 0 || b > len)
            throw;
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
        return String(n, result);
    }

    char &operator[](const unsigned int index) const {
        if (index >= len || index < 0) {
            throw;
        }
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

    long int getLen() const {
        return len;
    }

    char *getStr() const {
        return string;
    }

    friend istream &operator>>(istream &stream, String &instance) {
        delete[] instance.string;
        char *temp = new char[4097];
        stream.getline(temp, 4096, '\n');
        instance.len = strlen(temp);
        instance.string = new char[instance.len + 1];
        strncpy(instance.string, temp, instance.len + 1);
        delete[] temp;
        return stream;
    }

    friend ostream &operator<<(ostream &stream, String &instance) {
        stream << instance.string;
        return stream;
    }

    bool replace(const String &x, const String &y) {
        int count = 0, temp;
        for (int i = 0; i < len - x.getLen(); i++) {//находим количество вхождений подстроки
            temp = find_subs(string, x.getStr(), len, x.getLen(), i);
            if (-1 != temp) {
                count++;
                i = temp + x.getLen();
            }
        }
        if (!count)
            return false;
        //затем cnt раз должны заменить
        int shift = x.getLen() - y.getLen(); //сдвиг
        int newLen = len - count * shift + 1;//длина итоговой строчки
        //заупскаем цикл cnt раз, ищем вхождение строки перезаписывая её в результат, если нашли, то вписываем на её место другую подстроку
        char *res1 = new char[newLen];
//        char *X = new char[x.len];
        int tmp = 0, tmp2 = 0;
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
        return true;
    }

    ~String() {
        delete[] string;
    }

};

String operator+(const String &string1, const String &string2) {
    auto *string3 = new String(string1);
    *string3 += string2;
    return (*string3);
}

int main() {

//    ofstream out;
//    out.open("C:\\Users\\User\\CLionProjects\\proga\\input.txt");
    String str1, str2, str3;
    cin >> str1 >> str2 >> str3;
    str1.replace(str2, str3);
    cout << str1;
}
