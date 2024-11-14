#include "../src/numbers/rational.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>


string ration_operations()
{
    Rational r1("Frac[123456789, 123456789123456]");
    Rational r2("Frac[9876543, 1111111]");

    // Сравнение
    assert(r1 < r2);
    assert(!(r1 == r2));

    return "PASSED";
}

string test_ration_reduction()
{
    // Сокращение
    assert(Rational("Frac[987654321123456789, 123456789987654321]").reduce() == Rational("Frac[808080809, 101010101]"));
    assert(Rational("Frac[123456, 740736]").reduce() == Rational("Frac[1, 6]"));

    return "PASSED";
}

string is_rational_int()
{
    // Проверка на целое                                //пока не понимаю, как ловить исключение
    assert(!Rational("Frac[123456789123456789, 1234321345676]").is_int());
    assert(!Rational("Frac[9876543219876543219876, 12345432345678876543456]").is_int());

    assert(Rational("Frac[3086419728086419725, 123456789123456789]").is_int());
    assert(Rational("Frac[4766556655543322233455677, 1]").is_int());
    assert(Rational("Frac[0, 4988776654334456667]").is_int());
    assert(Rational("Frac[-4, 2]").is_int());

    return "PASSED";
}

string test_ration_division_by_zero()
{
    // Деление на ноль
    try
    {
        Rational("Frac[4, 0]");
        assert(false); // Исключение должно быть поймано
    }
    catch (const std::invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_ration_addition()
{
    assert(Rational("Frac[123456789, 987654321]") + Rational("Frac[987654321, 123456789]") == Rational("Frac[12230896747409402, 1505341124847349]"));
    assert(Rational("Frac[445677665433445, 988776543344556]") + Rational("Frac[0, 1]") == Rational("Frac[445677665433445, 988776543344556]"));
    assert(Rational("Frac[123456789987654321, 567]") + Rational("Frac[1, 1]") == Rational("Frac[13717421109739432, 63]"));
    assert(Rational("Frac[12, 45]") + Rational("Frac[87, 7]") == Rational("Frac[1333, 105]"));
    assert(Rational("Frac[12, 45]") + Rational("Frac[-87, 7]") == Rational("Frac[-1277, 105]"));
    assert(Rational("Frac[-4, 2]") + Rational("Frac[4, 3]") == Rational("Frac[-2, 3]"));
    assert(Rational("Frac[-4, 1]") + Rational("Frac[0, 1]") == Rational("Frac[-4, 1]"));

    return "PASSED";
}

string test_ration_sub()
{
    assert(Rational("Frac[1, 28]") - Rational("Frac[3, 14]") == Rational("Frac[-5, 28]"));
    assert(Rational("Frac[4, 2]") - Rational("Frac[0, 1]") == Rational("Frac[4, 2]"));
    assert(Rational("Frac[4, 3]") - Rational("Frac[1, 1]") == Rational("Frac[1, 3]"));
    assert(Rational("Frac[12, 45]") - Rational("Frac[87, 7]") == Rational("Frac[-1277, 105]"));
    assert(Rational("Frac[12, 45]") - Rational("Frac[-87, 7]") == Rational("Frac[1333, 105]"));
    assert(Rational("Frac[-4, 2]") - Rational("Frac[4, 3]") == Rational("Frac[-10, 3]"));
    assert(Rational("Frac[-4, 1]") - Rational("Frac[0, 1]") == Rational("Frac[-4, 1]"));

    return "PASSED";
}

string test_ration_mul()
{
    assert(Rational("Frac[1, 123456789123456789]") * Rational("Frac[3, 14]") == Rational("Frac[1, 576131682576131682]"));
    assert(Rational("Frac[4, 2]") * Rational("Frac[0, 1]") == Rational("Frac[0, 2]"));
    assert(Rational("Frac[4, 3]") * Rational("Frac[1, 1]") == Rational("Frac[4, 3]"));
    assert(Rational("Frac[12, 45]") * Rational("Frac[87, 7]") == Rational("Frac[1044, 315]"));
    assert(Rational("Frac[1, 123456789123456789]") * Rational("Frac[-3, 14]") == Rational("Frac[-1, 576131682576131682]"));
    assert(Rational("Frac[-1, 123456789123456789]") * Rational("Frac[3, 14]") == Rational("Frac[-1, 576131682576131682]"));

    return "PASSED";
}

string test_ration_div()
{
    assert(Rational("Frac[123456789123456789, 123123]") / Rational("Frac[123456789123456789, 123123]") == Rational("Frac[1, 1]"));
    assert(Rational("Frac[123456789123456789, 123123]") / Rational("Frac[123456789123456789, 1231234]") == Rational("Frac[1231234, 123123]"));
    assert(Rational("Frac[12, 45]") / Rational("Frac[87, 7]") == Rational("Frac[28, 1305]"));
    assert(Rational("Frac[12, 45]") / Rational("Frac[-87, 7]") == Rational("Frac[-28, 1305]"));
    assert(Rational("Frac[-123456789123456789, 123123]") / Rational("Frac[123456789123456789, 1231234]") == Rational("Frac[-1231234, 123123]"));

    return "PASSED";
}

int main()
{
    std::cout << "Запуск тестов..." << std::endl;
    std::cout << "Тест операций сравнения: " << ration_operations() << std::endl;
    std::cout << "Тест сокращения дробей: " << test_ration_reduction() << std::endl;
    std::cout << "Тест проверки на целое: " << is_rational_int() << std::endl;
    std::cout << "Тест деление на 0: " << test_ration_division_by_zero() << std::endl;
    std::cout << "Тест операции сложения: " << test_ration_addition() << std::endl;
    std::cout << "Тест оперпции вычитания: " << test_ration_sub() << std::endl;
    std::cout << "Тест операции умножения: " << test_ration_mul() << std::endl;
    std::cout << "Тест операции деления: " << test_ration_div() << std::endl;

    return 0;
}