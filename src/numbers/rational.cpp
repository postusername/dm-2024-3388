#include "rational.h"

// Конструктор с параметром по умолчанию
Rational::Rational(int64_t num) : Rational(Integer(num)) {}

// Конструктор, принимающий числитель и знаменатель
Rational::Rational(const Integer& numerator, const Natural& denominator) {
  data = std::make_pair(numerator, denominator);
  reduce();
}

/* TRANS_Z_Q Еникеев А. А. 3388 */
Rational::Rational(Integer num) {
  // Знаменатель num, числитель 1
  data = std::make_pair(num, Natural(1));
}

 // Конструктор, принимающий строку
Rational::Rational(std::string str) {
  data = from_string(str);
}

// Преобразование строки в пару (Integer, Natural)
std::pair<Integer, Natural> Rational::from_string(std::string str) {
  std::regex pattern(R"(Frac\[\s*(-?\d+)\s*,\s*(-?\d+)\s*\])"); // Frac[Integer, Integer]
  std::smatch matches;
  
  if (std::regex_search(str, matches, pattern) && matches.size() == 3) {
    // matches[1] - это числитель, matches[2] - это знаменатель
    Integer numerator = Integer(matches[1].str());
    Integer denominator = Integer(matches[2].str());
    // знаменатель не может быть 0
    if (denominator == Integer(0)) {
      throw std::invalid_argument("Denominator cannot be zero.");
    }
    // устанавливаем знак числителя
    if (denominator.sign() == -1) {
      numerator = -numerator;
    }

    return std::make_pair(numerator, denominator.abs());
  }
  
  throw std::invalid_argument("Input string is not in the expected format.");
}

/* RED_Q_Q Еникеев А. А. 3388 */
Rational& Rational::reduce() {
  // Получаем НОД числителя и знаменателя
  Natural gcf_num = gcf(data.first.abs(), data.second);
  // Сокращаем числитель
  data.first = data.first / Integer(gcf_num);
  // Сокращаем знаменатель
  data.second = data.second / gcf_num;
  return *this;
}

/* INT_Q_B Еникеев А.А. 3388 */
bool Rational::is_int() {
  Rational copy(to_string());
  // Сокращаем дробь
  copy.reduce();
  // Проверяем, является ли она целым числом
  return copy.get_denominator() == Natural(1);
}

// Получение числителя
Integer Rational::get_numerator() const {
  return data.first;
}

// Получение знаменателя
Natural Rational::get_denominator() const {
  return data.second;
}

// Строковое представление рационального числа 
std::string Rational::to_string() const {
    return "Frac[" + data.first.to_string() + ", " + data.second.to_string() + "]";
}

// Оператор сравнения < 
const bool operator<(const Rational& left, const Rational& right) {
  // Вычисляем числители с общим знаменателем и сравниваем числители
  auto [left_numerator, right_numerator, _] = 
      Rational::get_common_numerators_and_denominator(left, right);
  return left_numerator < right_numerator;
}

// Оператор сравнения ==
const bool operator==(const Rational& left, const Rational& right) {
  //  Вычисляем числители с общим знаменателем и сравниваем числители
  auto [left_numerator, right_numerator, _] = 
      Rational::get_common_numerators_and_denominator(left, right);
  return left_numerator == right_numerator;
}

/* TRANS_Q_Z Еникеев А. А. 3388 */
Rational::operator Integer() {
  if (!is_int()) {
    throw IntegerRangeException();
  }
  // Если знаменатель 1, возвращаем числитель
  return data.first;
}

/* ADD_QQ_Q Еникеев А.А. 3388 */ 
const Rational operator+(const Rational& left, const Rational& right) {
  // Вычисляем числители с общим знаменателем и складываем числители
  auto [left_numerator, right_numerator, denominator] = 
      Rational::get_common_numerators_and_denominator(left, right);
  return Rational(left_numerator + right_numerator, denominator);
}

/* SUB_QQ_Q Еникеев А. А. 3388 */
const Rational operator-(const Rational& left, const Rational& right) {
  //  Вычисляем числители с общим знаменателем и получаем разность числителей 
  auto [left_numerator, right_numerator, denominator] = 
      Rational::get_common_numerators_and_denominator(left, right);
  return Rational(left_numerator - right_numerator, denominator);
}

/* MUL_QQ_Q Еникеев А. А. 3388 */
const Rational operator*(const Rational& left, const Rational& right) {
  // Умножаем числители 
  Integer numerator = left.data.first * right.data.first;
  // Умножаем знаменатели 
  Natural denominator = left.data.second * right.data.second;
  return Rational(numerator, denominator);
}

/* DIV_QQ_Q Еникеев А. А. 3388 */
const Rational operator/(const Rational& left, const Rational& right) {
  // Проверка деления на 0
  if (right.data.first == Integer(0)) {
    throw std::invalid_argument("Division by zero");
  }
  // Перемножаем числители на знаменатели
  Integer numerator = left.data.first * Integer(right.data.second);
  Natural denominator = left.data.second * right.data.first.abs();
  // Устанавливаем знак числителя
  if (right.data.first.sign() == -1) {
    numerator = -numerator;
  }
  return Rational(numerator, denominator);
}

// Метод для приведения двух дробей к общему знаменателю 
std::tuple<Integer, Integer, Natural> Rational::get_common_numerators_and_denominator(
    const Rational& left, const Rational& right) {
  // Получаем НОК знаменателей
  Natural lcm_denominator = lcm(right.data.second, left.data.second);
  // Умножаем числители на множители для получения общего знаменателя дробей
  Integer left_numerator = left.data.first * Integer(lcm_denominator / left.data.second);
  Integer right_numerator = right.data.first * Integer(lcm_denominator / right.data.second);
  // Возвращаем числители и общий знаменатель 
  return {left_numerator, right_numerator, lcm_denominator};
}


