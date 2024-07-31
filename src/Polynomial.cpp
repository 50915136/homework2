#include <iostream>
#include <cmath> // 包含 cmath 以使用 pow 函數
using namespace std;

class Polynomial; // 前向宣告，告訴編譯器 Polynomial 是一個類別

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
private:
    float coef;
    int exp;
};

class Polynomial {
public:
    Polynomial();
    Polynomial(const Polynomial& poly);
    ~Polynomial();
    Polynomial add(const Polynomial& poly);
    Polynomial mult(const Polynomial& poly);
    float eval(float f) const;

    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;
    void resize(int newCapacity);
};

// Constructor
Polynomial::Polynomial() : capacity(10), terms(0) {
    termArray = new Term[capacity];
}

// Copy Constructor
Polynomial::Polynomial(const Polynomial& poly) : capacity(poly.capacity), terms(poly.terms) {
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = poly.termArray[i];
    }
}

// Destructor
Polynomial::~Polynomial() {
    delete[] termArray;
}

// Resize array if necessary
void Polynomial::resize(int newCapacity) {
    Term* newArray = new Term[newCapacity];
    for (int i = 0; i < terms; i++) {
        newArray[i] = termArray[i];
    }
    delete[] termArray;
    termArray = newArray;
    capacity = newCapacity;
}

// Add two polynomials
Polynomial Polynomial::add(const Polynomial& poly) {
    Polynomial result;
    int i = 0, j = 0;
    while (i < terms && j < poly.terms) {
        if (termArray[i].exp == poly.termArray[j].exp) {
            float coef = termArray[i].coef + poly.termArray[j].coef;
            if (coef != 0) {
                if (result.terms == result.capacity) {
                    result.resize(result.capacity * 2);
                }
                result.termArray[result.terms].coef = coef;
                result.termArray[result.terms++].exp = termArray[i].exp;
            }
            i++;
            j++;
        } else if (termArray[i].exp > poly.termArray[j].exp) {
            if (result.terms == result.capacity) {
                result.resize(result.capacity * 2);
            }
            result.termArray[result.terms++] = termArray[i++];
        } else {
            if (result.terms == result.capacity) {
                result.resize(result.capacity * 2);
            }
            result.termArray[result.terms++] = poly.termArray[j++];
        }
    }

    while (i < terms) {
        if (result.terms == result.capacity) {
            result.resize(result.capacity * 2);
        }
        result.termArray[result.terms++] = termArray[i++];
    }

    while (j < poly.terms) {
        if (result.terms == result.capacity) {
            result.resize(result.capacity * 2);
        }
        result.termArray[result.terms++] = poly.termArray[j++];
    }

    return result;
}

// Multiply two polynomials
Polynomial Polynomial::mult(const Polynomial& poly) {
    Polynomial result;
    for (int i = 0; i < terms; i++) {
        for (int j = 0; j < poly.terms; j++) {
            float coef = termArray[i].coef * poly.termArray[j].coef;
            int exp = termArray[i].exp + poly.termArray[j].exp;
            bool found = false;
            for (int k = 0; k < result.terms; k++) {
                if (result.termArray[k].exp == exp) {
                    result.termArray[k].coef += coef;
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (result.terms == result.capacity) {
                    result.resize(result.capacity * 2);
                }
                result.termArray[result.terms].coef = coef;
                result.termArray[result.terms++].exp = exp;
            }
        }
    }
    return result;
}

// Evaluate polynomial at a given value
float Polynomial::eval(float f) const {
    float result = 0;
    for (int i = 0; i < terms; i++) {
        result += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return result;
}

// Overloaded << operator
ostream& operator<<(ostream& os, const Polynomial& poly) {
    for (int i = 0; i < poly.terms; i++) {
        if (i > 0 && poly.termArray[i].coef > 0) os << " + ";
        os << poly.termArray[i].coef << "x^" << poly.termArray[i].exp;
    }
    return os;
}

// Overloaded >> operator
istream& operator>>(istream& is, Polynomial& poly) {
    cout << "Enter number of terms: ";
    is >> poly.terms;
    if (poly.terms > poly.capacity) {
        poly.resize(poly.terms);
    }
    for (int i = 0; i < poly.terms; i++) {
        cout << "Enter coefficient and exponent: ";
        is >> poly.termArray[i].coef >> poly.termArray[i].exp;
    }
    return is;
}

int main() {
    Polynomial p1, p2, p3;
    cout << "Enter first polynomial:\n";
    cin >> p1;
    cout << "Enter second polynomial:\n";
    cin >> p2;
    p3 = p1.add(p2);
    cout << "Sum: " << p3 << endl;
    p3 = p1.mult(p2);
    cout << "Product: " << p3 << endl;
    return 0;
}

