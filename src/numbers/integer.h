#ifndef DMITI__INTEGER_H_
#define DMITI__INTEGER_H_

#include <string>
#include <utility>
#include <iostream>
#include <cstdint>
#include <regex>
#include <limits>

#include "base.h"
#include "natural.h"

/*************************************
 * Целые числа                       *
 * Ижболдин Алесандр                 *
 * 3388                              *
 *************************************/

class Integer : public NumberInterface<std::pair<bool, Natural>> {
 public:
// Конструктор от целого числа int64_t
  Integer(int64_t num);
// TRANS_N_Z Ижболдин А.В 3388
  Integer(Natural num);

// Метод для преобразования строки в пару <bool, Natural>
  std::pair<bool, Natural> from_string(std::string str);

// ABS_Z_N Ижболдин А.В 3388
  Natural abs() const;

// SGN_Z_D Ижболдин А.В 3388
  int sign() const;

// Операторы сравнения
  friend const bool operator<(const Integer &left, const Integer &right);
  friend const bool operator==(const Integer &left, const Integer &right);

// MUL_ZM_Z Ижболдин А.В 3388
  friend const Integer operator-(const Integer &right);

// TRANS_Z_N Ижболдин А.В 3388
  explicit operator Natural() const;

// ADD_ZZ_Z Ижболдин А.В 3388
  friend const Integer operator+(const Integer &left, const Integer &right);
// SUB_ZZ_Z Ижболдин А.В 3388
  friend const Integer operator-(const Integer &left, const Integer &right);
// MUL_ZZ_Z Ижболдин А.В 3388
  friend const Integer operator*(const Integer &left, const Integer &right);
// DIV_ZZ_Z Ижболдин А.В 3388
  friend const Integer operator/(const Integer &left, const Integer &right);
// MOD_ZZ_Z Ижболдин А.В 3388
  friend const Integer operator%(const Integer &left, const Integer &right);

  std::string to_string() const;

 private:
  Integer(bool is_negative, Natural natural_value); // Приватный конструктор
};

#endif //DMITI__INTEGER_H_
