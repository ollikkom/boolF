#ifndef BOOLF_BOOLF_H
#define BOOLF_BOOLF_H

#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <exception>

class boolean_function {
public:
    using value_type = bool;
    using size_type = size_t;
    using reference = std::vector<value_type>::reference;
    using const_reference = std::vector<value_type>::const_reference;

    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    std::vector<value_type> func;
private:
    bool is_comparable(const boolean_function &a, const boolean_function &b) const {
        if (a.size() != b.size())
            return false;
        bool is_more = false;
        bool is_less = false;
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i] > b[i])
                is_more = true;
            else
                is_less = true;
            if (is_more && is_less)
                return false;
        }
        return true;
    }

public:
    static boolean_function var(size_t n, size_t dimension) {
        boolean_function bf(pow(2, dimension));
        bool change = false;
        for (size_t i = 0; i != pow(2, dimension); i + pow(2, n)) {
            if (change) {
                for (size_t j = 0; j < pow(2, n); ++j)
                    bf.func[i + j] = 1;
                change = false;
            } else
                change = true;
        }
        return bf;
    }

    static boolean_function zero(size_t dimension) {

        return boolean_function(dimension);
    }

    static boolean_function one(size_t dimension) {

        return boolean_function(pow(2, dimension) - 1, dimension);
    }

    static boolean_function from_anf(std::vector<value_type> v) {
        boolean_function bf;
        while (v.size() >= 1) {
            bf.func.push_back(v[0]);
            for (size_t i = 0; i < v.size() - 1; ++i) {
                v[i] = v[i] ^ v[i + 1];
            }
            v.pop_back();
        }
        return bf;
    }

    boolean_function() : func() {}


    boolean_function(size_t n) : func(pow(2, n), 0) {}

    // задает фунцию от n переменных. значение равно value
    // пример
    // пусть value = 14, т.е. 0...00001110 в двоичной системе
    // а n = 3
    // тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
    boolean_function(unsigned long long value, size_type n) {
        unsigned long long len = pow(2, n);
        for (size_t i = 0; i < pow(2, n); ++i) {
            size_t x = len & 1;
            func.push_back(x);
            len >> 1;
        }
    }

    // пусть table = "01110000"
    // тогда АНФ boolean_function будет равна x + y + xy + zx + zy + zyx
    boolean_function(const std::string &table) {
        if (log2(table.size()) != 2)
            throw std::logic_error(" !!! ");
        func.insert(this->end(), table.begin(), table.end());
    }

    // пусть table = {true, false, false, true};
    // тогда АНФ boolean_function будет равна x + y + 1
    boolean_function(const std::vector<value_type> &table) {
        func.insert(this->end(), table.begin(), table.end());
        if (log2(table.size()) != 2)
            throw std::logic_error("Error!");

    }

    boolean_function(const std::initializer_list<bool> vars) {
        if (log2(vars.size()) != 2)
            throw std::logic_error("Error!");
        func.insert(this->end(), vars.begin(), vars.end());

    }

    boolean_function(const boolean_function &table) {

        func = table.func;
    }

    boolean_function &operator=(const boolean_function &rhs) {
        if (&rhs == this)
            return *this;
        func = rhs.func;
        return *this;
    }

    // сложение по модулю 2
    // если разное количество переменных - исключение
    boolean_function &operator+=(const boolean_function &rhs) {
        if (this->size() != rhs.size())
            throw std::exception();
        for (size_t i = 0; i < this->size(); ++i) {
            func[i] = func[i] ^ rhs.func[i];
        }
        return *this;
    }

    // конъюнкция
    // если разное количество переменных - исключение
    boolean_function &operator*=(const boolean_function &rhs) {
        if (this->size() != rhs.size())
            throw std::exception();
        for (size_t i = 0; i < this->size(); ++i) {
            func[i] = func[i] & rhs.func[i];
        }
        return *this;
    }

    boolean_function &operator|=(const boolean_function &rhs) {
        if (this->size() != rhs.size())
            throw std::exception();
        for (size_t i = 0; i < this->size(); ++i) {
            func[i] = func[i] | rhs.func[i];
        }
        return *this;
    }

    boolean_function &operator~() {
        func.flip();
        return *this;
    }

    bool operator==(const boolean_function &rhs) const {
        if (this->size() != rhs.size())
            return false;
        for (auto i = 0; i < this->size(); ++i) {
            if (func[i] != rhs[i])
                return false;
        }
        return true;
    }

    bool operator>=(const boolean_function &rhs) const {
        if (!is_comparable(*this, rhs))
            throw std::logic_error("Error!");
        for (size_t i = 0; i < this->size(); ++i) {
            if (func[i] < rhs[i])
                return false;
            if (func[i] > rhs[i])
                return true;
        }
    }

    reference operator[](size_type ind) {

        return func[ind];
    }

    const_reference operator[](size_type ind) const {

        return func[ind];
    }

    reference at(size_type ind) {

        return func.at(ind);
    }

    const_reference at(size_type ind) const {
        return func.at(ind);
    }

    iterator begin() {

        return func.begin();
    }

    iterator end() {
        return func.end();
    }

    const_iterator cbegin() const {

        return func.cbegin();
    }

    const_iterator cend() const {

        return func.cend();
    }

    // длина столбца функции
    size_type size() const throw() {
        return func.size();
    }

    // количество переменных
    size_type dimension() const throw() {
        return log2(size());
    }

    // возвращает значение функции
    // пусть boolean_function задает функцию f(x, y, z)
    // тогда функция вернет f(vars[0], vars[1], vars[2])
    bool operator()(const std::vector<bool> &vars) const {
        size_t k = 0;
        for (size_t i = 0; i < vars.size(); ++i) {
            if (vars[vars.size() - 1 - i])
                k += pow(2, i);
        }
        return func[k];
    }

    bool operator()(const std::initializer_list<bool> vars) const {
        size_t k = 0;
        size_t i = 0;
        for (auto &v : vars) {
            if (v == true)
                k += pow(2, i);
            ++i;
        }
        return func[k];
    }


    // T(x1, x2, ..., xN) - текущая функция
    // operator вернет новую функцию, которая равна композиции G = T(fs[0], fs[1], ..., fs[N-1])
    boolean_function operator()(const std::vector<boolean_function> &fs) const {}

    boolean_function operator()(const std::initializer_list<boolean_function> vars) const {}

    bool is_monotone() const {
        size_t dist = this->size() / 2;
        while (dist > 0) {
            for (size_t i = 0; i < this->size(); i + 2 * dist) {
                for (size_t j = 0; j < dist; ++j) {
                    if (func[i + j] > func[i + j + dist])
                        return false;
                }
            }
            dist /= 2;
        }
        return true;
    }

    bool is_symmetric() const {
        std::vector<value_type> a;
        for (int i = this->size() - 1; i >= 1; --i) {
            a.push_back(func[i]);
        }
        a.flip();
        return func == a;

    }

    bool is_linear() const {
        std::vector<value_type> anff = anf();
        bool check = true;
        size_t ost = 0;
        size_t n = 0;
        for (int i = 0; i < anff.size(); i++) {
            ost = 0;
            n = i;
            while (n > 0) {
                ost += n % 2;
                n /= 2;
            }
            if (ost >= 1) {
                if (anff[i] != 0) {
                    check = false;
                    break;
                }
            }
        }
        return check;
    }

    bool is_T1() const {

        return func[size() - 1] == 1;
    }

    bool is_T0() const {

        return func[0] == 0;
    }

    bool is_balanced() const {
        size_t one = 0;
        size_t zero = 0;
        for (auto i = this->cbegin(); i != this->cend(); ++i) {
            if (*i == 1)
                ++one;
            else
                ++zero;
        }
        return zero == one;
    }

    size_t weight() const {
        size_t one = 0;
        for (auto i = this->cbegin(); i != this->cend(); ++i) {
            if (*i == 1)
                ++one;
        }
        return one;
    }

    bool is_functionally_complete_system() const {
        if (!is_T0() && !is_T1() && !is_symmetric() && !is_monotone() && !is_linear())
            return true;
        else
            return false;
    }

    // Возвращает АНФ функции
    std::vector<value_type> anf() const {}
};

// пусть boolean_function представляет из себя функцию "01110000"
// тогда get_polynom вернет строку "x0 + x1 + x0x1 + x0x2 + x1x2 + x0x1x2"
std::string get_polynom(const boolean_function &);

boolean_function operator+(const boolean_function &a, const boolean_function &b) {
    boolean_function bf(a);
    bf += b;
    return bf;
}

boolean_function operator*(const boolean_function &a, const boolean_function &b) {
    boolean_function bf(a);
    bf *= b;
    return bf;
}

boolean_function operator|(const boolean_function &a, const boolean_function &b) {
    boolean_function bf(a);
    bf |= b;
    return bf;
}

bool operator!=(const boolean_function &a, const boolean_function &b) {
    return !(a == b);
}

double log2(size_t x) {
    return log(x) / log(2);
}

#endif //BOOLF_BOOLF_H
