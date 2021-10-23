#include <iostream>
#include <cstring>

using namespace std;

class Expression {
public:
    virtual Expression *derivative(const string &var) = 0;

    virtual void print() = 0;

    virtual int eval(const string &varVal) = 0;

    virtual Expression *copy() = 0;

};

class Number : public Expression {
    int value;
public:
    Number(int value) : value(value) {}

    virtual Expression *copy() {
        return new Number(value);
    }

    Expression *derivative(const string &difVAr) {
        return new Number(0);
    }

    int eval(const string &varVal) {
        return value;
    }

    void print() {
        cout << value;
    }
};

class Variable : protected Expression {
    string var;
public:
    Variable(string var) : var(var) {}

    virtual Expression *copy() {
        return new Variable(var);
    }

    Expression *derivative(const string &difVar) {
        return new Number(1);
    }

    int eval(const string &varVal) {
        for (int i = 0; i < varVal.size(); i++)
            if (varVal[i] == var[0]) {
                int j = 0;
                for (; j < var.size(); j++)
                    if (varVal[i + j] != var[j])
                        break;
                if (j == var.size()) {
//                    return i;
                }
            }
    }

    void print() {
        cout << var;
    }
};

class Add : public Expression {
    Expression *a;
    Expression *b;
public:
    Add(Expression *a, Expression *b) : a(a), b(b) {}

    Expression *copy() {
        return new Add(a->copy(), b->copy());
    }

    Expression *derivative(const string &difVAr) {
        return new Add(a->derivative(difVAr), b->derivative(difVAr));
    }

    int eval(const string &varVal) {
        return a->eval(varVal) + b->eval(varVal);
    }

    void print() {
        cout << '(';
        a->print();
        cout << " + ";
        b->print();
        cout << ')';
    }

};

class Sub : public Expression { ;
    Expression *a;
    Expression *b;
public:
    Sub(Expression *a, Expression *b) : a(a), b(b) {}

    Expression *copy() {
        return new Sub(a->copy(), b->copy());
    }

    Expression *derivative(const string &difVar) {
        return new Sub(a->derivative(difVar), b->derivative(difVar));
    }

    int eval(const string &var) {
        return a->eval(var) - b->eval(var);
    }

    void print() {
        cout << '(';
        a->print();
        cout << " - ";
        b->print();
        cout << ')';
    }
};

class Mul : public Expression {
    Expression *a;
    Expression *b;
public:
    Mul(Expression *a, Expression *b) : a(a), b(b) {}

    Expression *copy() {
        return new Mul(a->copy(), b->copy());
    }

    Expression *derivative(const string &difVar) {
        return new Add(new Mul(a->derivative(difVar), b->copy()), new Mul(a->copy(), b->derivative(difVar)));
    }

    int eval(const string &varVal) {
        return a->eval(varVal) * b->eval(varVal);
    }

    void print() {
        cout << '(';
        a->print();
        cout << " * ";
        b->print();
        cout << ')';
    }
};

class Div : public Expression {
    Expression *a;
    Expression *b;
public:
    Div(Expression *a, Expression *b) : a(a), b(b) {}

    Expression *copy() {
        return new Div(a->copy(), b->copy());
    }

    int eval(const string &varVal) {
        return a->eval(varVal) / b->eval(varVal);
    }

    Expression *derivative(const string &difVar) {
        return new Div(new Sub(new Mul(a->derivative(difVar), b->copy()), new Mul(a->copy(), b->derivative(difVar))),
                       new Mul(b->copy(), b->copy()));
    }

    void print() {
        cout << '(';
        a->print();
        cout << " / ";
        b->print();
        cout << ')';
    }
};

int main() {

}