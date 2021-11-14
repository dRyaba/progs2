#include <iostream>
#include <cstring>
#include <fstream>

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



class String {

    int len;
    char *string;

    int find_subs(char *string1, const char *substring, size_t len1,
                  size_t sub_len,
                  int i) { //находит подстроку в строке и возвращает указатель на её начало, поиск идёт с i элемента
        for (; i < len1; i++)
            if (string1[i] == substring[0]) {
                int j = 0;
                for (; j < sub_len; j++)
                    if (string1[i + j] != substring[j])
                        break;
                if (j == sub_len) {
                    return i;
                }
            }
        return -1;
    }

public:
    String(int l, char *s) : len(l), string(s) {}

    String() : len(0), string(nullptr) {
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
            return *this;
        delete[] string;
        len = inst.len;
        string = new char[len + 1];
        strncpy(string, inst.string, len + 1);
        return *this;
    }

    bool operator==(const String &string1) {
        if (string1.getLen() != len)
            return false;
        for (int i = 0; i < len; ++i) {
            if (string[i] != string1.getStr()[i])
                return false;
        }
        return true;
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
            throw;
        char *result = new char[b - a + 2];
        for (int i = 0; i <= (b - a); i++)
            result[i] = string[a + i];
        result[b - a + 1] = 0;
        return {b - a + 2, result};
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

    int getLen() const {
        return len;
    }

    char *getStr() const {
        return string;
    }

    char *my_getline(std::ifstream &istream) {
        char *str = new char[1];
        str[0] = 0;
        char ch;
        char *buf;
        size_t length;

        while (istream.get(ch)) {
            if (ch == '\n') break;

            length = strlen(str);
            buf = new char[length + 2];
            strcpy(buf, str);
            buf[length] = ch;
            buf[length + 1] = 0;
            delete[]str;
            str = buf;

        }
        return str;
    }

    friend istream &operator>>(ifstream &stream, String &instance) {
        delete[] instance.string;
        char *temp = instance.my_getline(stream);
//        stream.getline(temp, 4096, '\n');
//        fgets(temp,126);
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
        int count = 0;
        size_t temp;
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
        size_t shift = x.getLen() - y.getLen(); //сдвиг
        int newLen = len - count * shift + 1;//длина итоговой строчки
        //заупскаем цикл cnt раз, ищем вхождение строки перезаписывая её в результат, если нашли, то вписываем на её место другую подстроку
        char *res1 = new char[newLen];
//        char *X = new char[x.len];
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
        return true;
    }

    ~String() {
        delete[] string;
    }

};

String operator+(const String &string1, const String &string2) {
    String temp = string1;
    temp += string2;
    return (temp);
}

class Expression {
public:
    virtual Expression *derivative(const String &var) = 0;

    virtual void print(ofstream &stream) = 0;

    virtual int eval(const String &varVal) = 0;

    virtual Expression *copy() = 0;

    virtual ~Expression() = default;

};

class Operation : public Expression {
protected:
    Expression *a, *b;
    char sign;

    Operation(Expression *a, Expression *b, char sign) : a(a), b(b), sign(sign) {}

    void print(std::ofstream &stream) {
        stream << '(';
        a->print(stream);
        stream << sign;
        b->print(stream);
        stream << ')';
    }

    ~Operation() {
        delete a;
        delete b;
    }
};

class Number : public Expression {
    int value;
public:
    Number(int value) : value(value) {}

    Expression *copy() {
        return new Number(value);
    }

    Expression *derivative(const String &difVAr) {
        return new Number(0);
    }

    int eval(const String &varVal) {
        return value;
    }

    void print(std::ofstream &stream) {
        stream << value;
    }
};

class Variable : public Expression {
    String var;
public:
    Variable(String var) : var(var) {}

    Expression *copy() {
        return new Variable(var);
    }

    Expression *derivative(const String &difVar) {
        for (int i = 0; difVar.getStr()[i] != '\0' && var.getStr()[i] != '\0'; i++) {
            if (var.getStr()[i] != difVar.getStr()[i])
                return new Number(0);
        }
        return new Number(1);
    }

    int eval(const String &varVal) {
        for (int i = 0; i < varVal.getLen() - 4; i++)
            if (varVal.getStr()[i] == var[0]) {
//                String a = varVal(i, i + var.getLen() - 2);
                if (varVal(i, i + var.getLen() - 2) == var) {
                    int res = 0, j = i, k = 5;
                    while ((j + k < varVal.getLen()) && varVal.getStr()[j + k] != ';') {
                        res *= 10;
                        res += varVal.getStr()[j + k] - '0';
                        k++;
                    }
                    return res;
                }
            }

        return 0;
    }

    void print(std::ofstream &stream) {
        stream << var;
    }
};


class Add : public Operation {
public:
    Add(Expression *a, Expression *b) : Operation(a, b, '+') {}

    Expression *copy() {
        return new Add(a->copy(), b->copy());
    }

    Expression *derivative(const String &difVAr) {
        return new Add(a->derivative(difVAr), b->derivative(difVAr));
    }

    int eval(const String &varVal) {
        return a->eval(varVal) + b->eval(varVal);
    }

    ~Add() {
        delete a;
        delete b;
    }
};

class Sub : public Operation { ;
public:
    Sub(Expression *a, Expression *b) : Operation(a, b, '-') {}

    Expression *copy() {
        return new Sub(a->copy(), b->copy());
    }

    Expression *derivative(const String &difVar) {
        return new Sub(a->derivative(difVar), b->derivative(difVar));
    }

    int eval(const String &var) {
        return a->eval(var) - b->eval(var);
    }

    ~Sub() {
        delete a;
        delete b;
    }
};

class Mul : public Operation {
public:
    Mul(Expression *a, Expression *b) : Operation(a, b, '*') {}

    Expression *copy() {
        return new Mul(a->copy(), b->copy());
    }

    Expression *derivative(const String &difVar) {
        return new Add(new Mul(a->derivative(difVar), b->copy()), new Mul(a->copy(), b->derivative(difVar)));
    }

    int eval(const String &varVal) {
        return a->eval(varVal) * b->eval(varVal);
    }


    ~Mul() {
        delete a;
        delete b;
    }
};

class Div : public Operation {
public:
    Div(Expression *a, Expression *b) : Operation(a, b, '/') {}

    Expression *copy() {
        return new Div(a->copy(), b->copy());
    }

    int eval(const String &varVal) {
        return a->eval(varVal) / b->eval(varVal);
    }

    Expression *derivative(const String &difVar) {
        return new Div(new Sub(new Mul(a->derivative(difVar), b->copy()), new Mul(a->copy(), b->derivative(difVar))),
                       new Mul(b->copy(), b->copy()));
    }


    ~Div() {
        delete a;
        delete b;
    }
};

Expression *parse(const String &exp, int start, int *i) {
    if (exp.getStr()[start] == '(') {
        (*i)++;
        Expression *first = parse(exp.getStr(), start + 1, i);
        char sign = exp.getStr()[*i];
        (*i)++;
        Expression *second = parse(exp.getStr(), *i, i);
        (*i)++;
        switch (sign) {
            case '+':
                return new Add(first, second);
            case '-':
                return new Sub(first, second);
            case '*':
                return new Mul(first, second);
            case '/':
                return new Div(first, second);
            default:
                return nullptr;
        }
    } else {
        if (isdigit(exp.getStr()[start])) {
            int digLen = 0;
            int res = 0;
            while (isdigit(exp.getStr()[start + digLen])) {
                res *= 10;
                res += exp.getStr()[start + digLen] - '0';
                digLen++;
            }
            *i += digLen;
            return new Number(res);
        }
        if (isalpha(exp.getStr()[start])) {
            int varLen = 0;
            String res = "";
            while (isalpha(exp.getStr()[*i + varLen])) {
                res += exp.getStr()[*i + varLen];
                varLen++;
            }
            *i += varLen;
            return new Variable(res);
        }
        return nullptr;
    }
}

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    String expression;
    in >> expression;
    int i = 0;
    Expression *e = parse(expression, 0, &i);
    Expression *b = e->derivative("x");
    b->print(out);
//    out << e->eval("x <- 5");
}