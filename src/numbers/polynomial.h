#pragma once

#include "base.h"
#include "natural.h"
#include "integer.h"
#include "rational.h"
#include <iostream>
#include <map>
#include <regex>
#include <stdexcept>
#include <string>
#include <map>

/*************************************
 * Многочлен с рациональными         *
 * коэффициентами                    *
 * Лутфулин Даниил                   *
 * 3388                              *
 *************************************/

class Polynomial : public NumberInterface<std::map<Natural, Rational>>
{
public:
    // using NumberInterface<std::map<Natural, Rational>>::NumberInterface;
    Polynomial(const Rational num = Rational(Integer(0)));
    Polynomial(std::initializer_list<std::pair<Natural, Rational>> num);
    Polynomial(std::string str);
    std::string to_string();

    std::map<Natural, Rational> from_string(std::string str);

    Polynomial &operator+=(const Polynomial &other);
    Polynomial &operator+=(const Rational &other);
    Polynomial &operator-=(const Polynomial &other);
    Polynomial &operator-=(const Rational &num);
    friend const Polynomial operator+(const Polynomial &left, const Polynomial &right);
    friend const Polynomial operator+(const Polynomial &poly, const Rational &num);
    friend const Polynomial operator+(const Rational &num, const Polynomial &poly);
    friend const Polynomial operator-(const Polynomial &left, const Polynomial &right);
    friend const Polynomial operator-(const Polynomial &poly, const Rational &num);
    friend const Polynomial operator-(const Rational &num, const Polynomial &poly);
    friend const bool operator==(const Polynomial &left, const Polynomial &right);
    Polynomial &operator*=(const Rational &num);
    friend const Polynomial operator*(const Polynomial &poly, const Rational &num);
    friend const Polynomial operator*(const Rational &num, const Polynomial &poly);
    Polynomial &mul_powX(const Natural k);
    Rational lead() const;
    Natural degree() const;
    bool isZero();
    Rational canonical();
    Polynomial &operator*=(const Polynomial &poly);
    friend const Polynomial operator*(const Polynomial &left, const Polynomial &right);
    friend const Polynomial operator/(const Polynomial &left, const Polynomial &right);
    friend const Polynomial operator%(const Polynomial &left, const Polynomial &right);
    friend Polynomial derivative(const Polynomial &X);
};


Polynomial gcf(Polynomial a, Polynomial b);
Polynomial normalize(const Polynomial &X);
