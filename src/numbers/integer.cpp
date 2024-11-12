#include "integer.h"

/*************************************
 * Целые числа                       *
 * Ижболдин Алесандр                 *
 * 3388                              *
 *************************************/

// Конструктор от целого числа int64_t
Integer::Integer(int64_t num) {
  if (num < 0) {
    data = {true, Natural(-num)};
  } else {
    data = {false, Natural(num)};
  }
}

// TRANS_N_Z Ижболдин А.В 3388
Integer::Integer(Natural num) : Integer(false, num) {}

// Конструктор от строки
Integer::Integer(std::string str) : NumberInterface<std::pair<bool, Natural>>(str) {}


// ABS_Z_N Ижболдин А.В 3388
Natural Integer::abs() const {
  return data.second;
}

// SGN_Z_D Ижболдин А.В 3388
int Integer::sign() const {
  return data.first ? -1 : 1;
}

// Оператор сравнения <
const bool operator<(const Integer &left, const Integer &right) {
  if (left.data.first != right.data.first) {
    return left.data.first;
  }
  if (left.data.first) {
    return right.data.second < left.data.second;
  }
  return left.data.second < right.data.second;
}

// Оператор сравнения ==
const bool operator==(const Integer &left, const Integer &right) {
  return left.data.first == right.data.first && left.data.second == right.data.second;
}

// MUL_ZM_Z Ижболдин А.В 3388
const Integer operator-(const Integer &right) {
  return Integer(!right.data.first, right.data.second);
}

// TRANS_Z_N Ижболдин А.В 3388
Integer::operator Natural() const {
  if (data.first) throw std::invalid_argument("Cannot convert negative Integer to Natural");
  return data.second;
}

// ADD_ZZ_Z Ижболдин А.В 3388
const Integer operator+(const Integer &left, const Integer &right) {
  if (left.data.first == right.data.first) {
    return Integer(left.data.first, left.data.second + right.data.second);
  } else if (left.data.second < right.data.second) {
    return Integer(right.data.first, right.data.second - left.data.second);
  } else {
    return Integer(left.data.first, left.data.second - right.data.second);
  }
}

// SUB_ZZ_Z Ижболдин А.В 3388
const Integer operator-(const Integer &left, const Integer &right) {
  return left + (-right);
}

// MUL_ZZ_Z Ижболдин А.В 3388
const Integer operator*(const Integer &left, const Integer &right) {
  bool result_sign = left.data.first != right.data.first;
  return Integer(result_sign, left.data.second * right.data.second);
}

// DIV_ZZ_Z Ижболдин А.В 3388
const Integer operator/(const Integer &left, const Integer &right) {
  if (right.data.second == Natural(0)) throw std::invalid_argument("Division by zero");
  bool result_sign = left.data.first != right.data.first;
  return Integer(result_sign, left.data.second / right.data.second);
}

// MOD_ZZ_Z Ижболдин А.В 3388
const Integer operator%(const Integer &left, const Integer &right) {
  if (right.data.second == Natural(0)) throw std::invalid_argument("Modulo by zero");
  bool result_sign = left.data.first;
  return Integer(result_sign, left.data.second % right.data.second);
}

// Приватный конструктор для создания Integer с определённым знаком
Integer::Integer(bool is_negative, Natural natural_value) {
  data = std::make_pair(is_negative, natural_value);
}

// Метод для преобразования строки в пару <bool, Natural>
pair<bool, Natural> Integer::from_string(std::string str) {
  std::regex pattern(R"(([\-])?(\d+))");
  std::smatch matches;

  if (std::regex_search(str, matches, pattern) && matches.size() == 3) {
    // matches[1] - это знак, matches[2] - это натуральное
    bool sign = true;
    if (matches[1].str().empty())
      sign = false;
    Natural num = Natural(matches[2].str());
    return std::pair<bool, Natural>(sign, num);
  }
  throw std::invalid_argument("Input string is not in the expected format.");
}

std::string Integer::to_string() const {
  return data.first ? "-" : "" + data.second.to_string();
}
