#include "../src/numbers/polynomial.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>


string test_polinom_addition()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") + Polynomial("Poly[[Frac[-3,2], 3], [Frac[5,2], 2], [-6,1], [15,0]]") == Polynomial("Poly[[Frac[5,1], 2], [25,0]]") + Rational(0));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") + Polynomial("Poly[[Frac[-3,2], 3], [Frac[-5,2], 2], [-6,1], [-10,0]]") == Polynomial("Poly[[Frac[0,1], 0]]"));
    assert(Polynomial("Poly[[Frac[1234, 5678], 2]]") + Polynomial("Poly[[Frac[0,1], 0]]") == Polynomial("Poly[[Frac[1234, 5678], 2]]"));

    return "PASSED";
}

string test_polinom_sub()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") - Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2]]") == Polynomial("Poly[[Frac[6,1], 1], [10,0]]"));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") - Polynomial("Poly[[Frac[-3,2], 3], [Frac[-5,2], 2], [-6,1], [Frac[2,3], 0]]") == Polynomial("Poly[[Frac[3,1], 3], [Frac[5,1], 2], [Frac[12,1], 1], [Frac[28,3], 0]]"));
    assert(Polynomial("Poly[[Frac[1234, 5678], 2]]") - Polynomial("Poly[[Frac[0,1], 0]]") == Polynomial("Poly[[Frac[1234, 5678], 2]]"));

    return "PASSED";
}
string test_mul_polinom_N()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Rational("Frac[987654321, 12]") == Polynomial("Poly[[Frac[987654321,8], 3], [Frac[1646090535,8], 2], [Frac[987654321,2], 1], [Frac[1646090535,2], 0]]"));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Rational("Frac[1, 1]") == Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]"));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Rational("Frac[-987654321, 12]") == Polynomial("Poly[[Frac[-987654321,8], 3], [Frac[-1646090535,8], 2], [Frac[-987654321,2], 1], [Frac[-1646090535,2], 0]]"));
    return "PASSED";
}

string test_MUL_Pxk_P()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Polynomial("Poly[[Frac[24682468,12341234], 40]]") == Polynomial("Poly[[Frac[3,1], 43], [Frac[5,1], 42], [Frac[12,1], 41], [Frac[20,1], 40]]"));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Polynomial("Poly[[Frac[24682468,12341234], 0]]") == Polynomial("Poly[[Frac[3,1], 3], [Frac[5,1], 2], [Frac[12,1], 1], [Frac[20,1], 0]]"));

    return "PASSED";
}

string test_led()
{
    assert(Polynomial("Poly[[Frac[3,2], 305], [Frac[5,2], 2], [6,1], [10,0]]").lead() == Rational("Frac[3, 2]"));
    assert(Polynomial("Poly[[Frac[3,2], 1], [Frac[5,2], 2], [Frac[6, 1], 3], [10, 0]]").lead() == Rational("Frac[6, 1]"));

    return "PASSED";
}

string test_deg()
{
    assert(Polynomial("Poly[[Frac[3,2], 305], [Frac[5,2], 2], [6,1], [10,0]]").degree() == Natural(305));
    assert(Polynomial("Poly[[Frac[3,2], 1], [Frac[5,2], 2], [Frac[6, 1], 3], [10, 0]]").degree() == Natural(3));

    return "PASSED";
}

string test_FAC_P_Q()
{ // приведение коэффицентов к каноничному виду (вынесение НОД числителей и НОК знаменателей)
    assert(Polynomial("Poly[[Frac[3,123], 300], [Frac[1,41], 150]]").canonical() == Rational("Frac[1, 123]"));
    assert(Polynomial("Poly[[Frac[3, 123], 300], [Frac[1,3], 150], [Frac[1, 5], 1]]").canonical() == Rational("Frac[1, 615]"));

    return "PASSED";
}

string test_polinom_mul()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") * Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") == Polynomial("Poly[[Frac[9,4], 6], [Frac[15,2], 5], [Frac[97,4], 4], [60,3], [86,2], [120,1], [100,0]]"));
    assert(Polynomial("Poly[[Frac[5,2], 2], [Frac[3,2], 3], [6,1], [10,0]]") * Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") == Polynomial("Poly[[Frac[9,4], 6], [Frac[15,2], 5], [Frac[97,4], 4], [60,3], [86,2], [120,1], [100,0]]"));
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6, 1], [10,0]]") * Polynomial("Poly[[Frac[5, 9], 2], [Frac[1, 1], 1]]") == Polynomial("Poly[[Frac[0, 1], 0], [Frac[10, 1], 1], [Frac[104, 9], 2], [Frac[35, 6], 3], [Frac[26, 9], 4], [Frac[5, 6], 5]]"));

    return "PASSED";
}

string test_polinom_div()
{
    assert(Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") / Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") == Polynomial("Poly[[Frac[1,1], 0]"));
    assert(Polynomial("Poly[[Frac[5,2], 2], [Frac[3,2], 3], [6,1], [10,0]]") / Polynomial("Poly[[Frac[3,2], 3], [Frac[5,2], 2], [6,1], [10,0]]") == Polynomial("Poly[[Frac[1,1], 0]"));

    return "PASSED";
}

int main()
{
    std::cout << "Запуск тестов..." << std::endl;
    std::cout << "Тест операций сложения полиномов: " << test_polinom_addition() << std::endl;
    std::cout << "Тест рперации вычитания полиномов: " << test_polinom_sub() << std::endl;
    std::cout << "Тест умножения полинома на число: " << test_mul_polinom_N() << std::endl;
    std::cout << "Тест умножения полинома на rx^n: " << test_MUL_Pxk_P() << std::endl;
    std::cout << "Тест определения старшего коэффициента полинома: " << test_led() << std::endl;
    std::cout << "Тест определения степени полинома: " << test_deg() << std::endl;
    std::cout << "Тест приведения коэффициентов к каноничному виду: " << test_FAC_P_Q() << std::endl;
    std::cout << "Тест умножения полиномов: " << test_polinom_mul() << std::endl;
    std::cout << "Тест деления полиномов: " << test_polinom_mul() << std::endl;

    return 0;
}