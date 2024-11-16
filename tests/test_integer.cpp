#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/numbers/integer.h"


string test_from_string_valid()
{
    Integer integer("5");
    std::string test("5");
    std::pair<bool, Natural> result = integer.from_string(test);
    assert(result.first == false);
    assert(result.second == 5);

    return "PASSED";
}

string test_from_string_zero()
{
    Integer integer("0");
    std::string test("0");
    std::pair<bool, Natural> result = integer.from_string(test);
    assert(result.first == false);
    assert(result.second == 0);

    return "PASSED";
}

string test_from_string_negative()
{
    Integer integer("-5");
    std::string test("-5");
    std::pair<bool, Natural> result = integer.from_string(test);
    assert(result.first == true);
    // Проверка, что Natural корректно обрабатывает отрицательные значения
    assert(result.second == 5);

    return "PASSED";
}

string test_from_string_empty()
{
    try
    {
        Integer result = Integer("");
    }
    catch (exception e)
    {
        return "PASSED";
    }

    assert(false);
}

string test_abs()
{
    assert(Integer(5).abs() == Natural(5));
    assert(Integer(-5).abs() == Natural(5));
    assert(Integer(0).abs() == Natural(0));

    return "PASSED";
}

string test_sign()
{
    assert(Integer(5).sign() == 1);
    assert(Integer(-5).sign() == -1);
    assert(Integer(0).sign() == 0);

    return "PASSED";
}

string test_operator_inv()
{
    Integer integer1(5);
    assert((-integer1).to_string() == "-5");

    Integer integer2(-5);
    assert((-integer2).to_string() == "5");

    Integer integer3(0);
    assert((-integer3).to_string() == "0");

    return "PASSED";
}

string test_operator_natural_valid()
{
    Integer integer(5);
    try
    {
        Natural natural = Natural(integer);
        assert(natural == Natural(5));
    }
    catch (const exception &ex)
    {
        assert(false);
    }

    return "PASSED";
}

string test_operator_natural_invalid()
{
    Integer integer(-5);
    try
    {
        Natural natural = Natural(integer);
        assert(false);
    }
    catch (const exception &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_operator_plus()
{
    assert(Integer(5) + Integer(10) == Integer(15));
    assert(Integer(5) + Integer(-10) == Integer(-5));
    assert(Integer(-5) + Integer(10) == Integer(5));
    assert(Integer(-5) + Integer(-10) == Integer(-15));
    assert(Integer(1230) + Integer(4560) == Integer(5790));
    assert(Integer(1230) + Integer(-4560) == Integer(-3330));
    assert(Integer(-1230) + Integer(4560) == Integer(3330));
    assert(Integer(-1230) + Integer(-4560) == Integer(-5790));
    assert(Integer(0) + Integer(10) == Integer(10));
    assert(Integer(10) + Integer(-10) == Integer(0));

    return "PASSED";
}

string test_operator_minus()
{
    assert(Integer(5) - Integer(10) == Integer(-5));
    assert(Integer(5) - Integer(-10) == Integer(15));
    assert(Integer(-5) - Integer(10) == Integer(-15));
    assert(Integer(-5) - Integer(-10) == Integer(5));
    assert(Integer(1230) - Integer(4560) == Integer(-3330));
    assert(Integer(1230) - Integer(-4560) == Integer(5790));
    assert(Integer(-1230) - Integer(4560) == Integer(-5790));
    assert(Integer(-1230) - Integer(-4560) == Integer(3330));
    assert(Integer(0) - Integer(10) == Integer(-10));
    assert(Integer(0) - Integer(-10) == Integer(10));
    assert(Integer(-10) - Integer(-10) == Integer(0));

    return "PASSED";
}

string test_operator_multiply()
{
    assert(Integer(5) * Integer(10) == Integer(50));
    assert(Integer(5) * Integer(-10) == Integer(-50));
    assert(Integer(-5) * Integer(10) == Integer(-50));
    assert(Integer(-5) * Integer(-10) == Integer(50));
    assert(Integer(123) * Integer(456) == Integer(56088));
    assert(Integer(123) * Integer(-456) == Integer(-56088));
    assert(Integer(-123) * Integer(456) == Integer(-56088));
    assert(Integer(-123) * Integer(-456) == Integer(56088));
    assert(Integer(0) * Integer(10) == Integer(0));
    assert(Integer(0) * Integer(-10) == Integer(0));

    return "PASSED";
}

string test_operator_divide()
{
    assert(Integer(10) / Integer(2) == Integer(5));
    assert(Integer(10) / Integer(-2) == Integer(-5));
    assert(Integer(-10) / Integer(2) == Integer(-5));
    assert(Integer(-10) / Integer(-2) == Integer(5));
    assert(Integer(0) / Integer(-2) == Integer(0));
    assert(Integer(11) / Integer(2) == Integer(5));   // Округление вниз
    assert(Integer(11) / Integer(-2) == Integer(-5)); // Округление вниз
    assert(Integer(-11) / Integer(2) == Integer(-5)); // Округление вниз
    assert(Integer(-11) / Integer(-2) == Integer(5)); // Округление вниз

    try
    {
        Integer(10) / Integer(0);
        assert(false); // Исключение должно быть поймано
    }
    catch (const std::invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_operator_modulo()
{
    assert(Integer(10) % Integer(2) == Integer(0));
    assert(Integer(10) % Integer(-2) == Integer(0));
    assert(Integer(-10) % Integer(2) == Integer(0));
    assert(Integer(-10) % Integer(-2) == Integer(0));
    assert(Integer(11) % Integer(2) == Integer(1));
    assert(Integer(0) % Integer(2) == Integer(0));
    assert(Integer(11) % Integer(-2) == Integer(1));
    assert(Integer(-11) % Integer(3) == Integer(1));
    assert(Integer(-7) % Integer(-3) == Integer(2));

    try
    {
        Integer(10) % Integer(0);
        assert(false); // Исключение должно быть поймано
    }
    catch (const std::invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

int main()
{
    std::cout << "Запуск тестов..." << std::endl;
    std::cout << "Тест 1: " << test_from_string_valid() << std::endl;
    std::cout << "Тест 2: " << test_from_string_zero() << std::endl;
    std::cout << "Тест 3: " << test_from_string_negative() << std::endl;
    std::cout << "Тест 4: " << test_from_string_empty() << std::endl;
    std::cout << "Тест 5: " << test_abs() << std::endl;
    std::cout << "Тест 6: " << test_sign() << std::endl;
    std::cout << "Тест 7: " << test_operator_inv() << std::endl;
    std::cout << "Тест 8: " << test_operator_natural_valid() << std::endl;
    std::cout << "Тест 9: " << test_operator_natural_invalid() << std::endl;
    std::cout << "Тест 10: " << test_operator_plus() << std::endl;
    std::cout << "Тест 11: " << test_operator_minus() << std::endl;
    std::cout << "Тест 12: " << test_operator_multiply() << std::endl;
    std::cout << "Тест 13: " << test_operator_divide() << std::endl;
    std::cout << "Тест 14: " << test_operator_modulo() << std::endl;

    return 0;
}