#pragma once
#include "base.h"


class Natural : public NumberInterface<vector<Digit>>, private rel_ops::make_rel_ops_work
{
public:
    Natural() = default;
    Natural(string str)
    {
        this->data = from_string(str);
    }
    Natural(uint64_t num) : Natural(to_string(num)) {}
    friend ostream &operator<<(ostream &stream, Natural &num);
    vector<Digit> from_string(string str);
    string to_string(uint64_t num);
    string to_string(vector<Digit> num);
    string to_string();
    int compare(Natural num) const;
    friend const bool operator<(const Natural &left, const Natural &right);
    friend const bool operator==(const Natural &left, const Natural &right);
    bool is_zero();
    Natural operator++();
    Natural operator++(int);
    void carry_flex(size_t i);
    void zero_flex();
    friend const Natural operator+(const Natural &left, const Natural &right);
    friend const Natural operator-(const Natural &left, const Natural &right);
    friend const Natural operator*(const Natural &num, const Digit &digit);
    friend const Natural operator*(const Digit &digit, const Natural &num);
    Natural &mul_pow10(const Natural k);
    friend const Natural operator*(const Natural &left, const Natural &right);
    Natural &sub_nmul(const Natural &n, const Digit &d);
    Natural &div_nmul(const Natural &n, const Natural k);
    friend const Natural operator/(const Natural &left, const Natural &right);
    friend const Natural operator%(const Natural &left, const Natural &right);

protected:
    vector<Digit> data;
};

Natural gcf(const Natural &a, const Natural &b);
Natural lcm(const Natural &a, const Natural &b);
