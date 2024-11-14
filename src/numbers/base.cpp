#include "base.h"


Digit::Digit(int d)
{
    this->d = d % 10;
    this->carry = d / 10;
}


int Digit::compare(Digit d) const
{
    if (this->d > d.d)
        return 2;
    if (this->d < d.d)
        return 1;
    return 0;
}


const bool operator<(const Digit &left, const Digit &right)
{
    return left.compare(right) == 1;
}


const bool operator==(const Digit &left, const Digit &right)
{
    return left.compare(right) == 0;
}


const Digit operator+(const Digit &left, const Digit &right)
{
    return Digit(left.d + right.d);
}


const Digit operator-(const Digit &left, const Digit &right)
{
    if (left.d < right.d)
        throw invalid_argument("Left value must be greater or equal of right value");
    return Digit(left.d - right.d);
}


const Digit operator*(const Digit &left, const Digit &right)
{
    return Digit((int)(left.d * right.d));
}


ostream &operator<<(ostream &stream, const Digit &digit)
{
    stream << digit.d;
    return stream;
}
