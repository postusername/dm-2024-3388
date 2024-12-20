#ifndef RATIONAL_H
#define RATIONAL_H

#include <string>
#include <tuple>
#include <utility>
#include <regex>
#include <stdexcept>
#include <cstdint>

#include "integer.h"
#include "natural.h"
#include "base.h"

/*************************************
 * Рациональные числа (дроби)        *
 * Еникеев Антон                     *
 * 3388                              *
 *************************************/

class Rational : public NumberInterface<std::pair<Integer, Natural>> {
 public:
  // Конструкторы
  Rational(int64_t num = 0);
  Rational(const Integer& numerator, const Natural& denominator);
  Rational(Integer num); // TRANS_Z_Q
  Rational(std::string str);

  std::pair<Integer, Natural> from_string(std::string& str);

  Rational& reduce(); // RED_Q_Q
  bool is_int(); // INT_Q_B
  
  explicit operator Integer(); // TRANS_Q_Z

  // Геттеры для числителя и знаменателя
  Integer get_numerator();
  Natural get_denominator();

  // Метод для получения строкового представления
  std::string to_string();

  // Операторы сравнения
  friend const bool operator<(const Rational& left, const Rational& right);
  friend const bool operator==(const Rational& left, const Rational& right);

  // Арифметические операторы
  friend const Rational operator+(const Rational& left, const Rational& right); // ADD_QQ_Q
  friend const Rational operator-(const Rational& left, const Rational& right); // SUB_QQ_Q
  friend const Rational operator*(const Rational& left, const Rational& right); // MUL_QQ_Q
  friend const Rational operator/(Rational left, Rational right); // DIV_QQ_Q
  
  friend ostream &operator<<(ostream &stream, Rational num);
 private:
  // Метод для приведения двух дробей к общему знаменателю
  static std::tuple<Integer, Integer, Natural> get_common_numerators_and_denominator(
      const Rational& left, const Rational& right);
};

#endif  // RATIONAL_H
