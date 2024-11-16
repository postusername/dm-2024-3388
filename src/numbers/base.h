#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


using namespace std;
using namespace std::rel_ops;

class Digit
{
public:
    Digit(int d);
    Digit() : d(0), carry(0) {}
    Digit(string str) : Digit(str[0] - '0') {}
    int compare(Digit d) const;
    friend const bool operator<(const Digit &left, const Digit &right);
    friend const bool operator==(const Digit &left, const Digit &right);
    friend const Digit operator+(const Digit &left, const Digit &right);
    friend const Digit operator-(const Digit &left, const Digit &right);
    friend const Digit operator*(const Digit &left, const Digit &right);
    friend const Digit operator/(const Digit &left, const Digit &right);
    friend const Digit operator%(const Digit &left, const Digit &right);
    friend ostream &operator<<(ostream &stream, const Digit &digit);

    int d;
    int carry;
};

template <typename T>
class NumberInterface
{
public:
    NumberInterface() = default;
    NumberInterface(string str)
    {
        this->data = from_string(str);
    }
    virtual T from_string(string& str) = 0;
    virtual string to_string() = 0;
    friend ostream &operator<<(ostream &stream, NumberInterface<T>& num)
    {
        stream << num.to_string();
        return stream;
    }

protected:
    T data;
};
