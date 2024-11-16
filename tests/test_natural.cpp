#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../src/numbers/natural.h"

using namespace std;


string test_compare()
{ // компаратор натуральных чисел (возвращаемое значение нестандартно!!)
    Natural n1(5), n2(5), n3(10);
    assert(n1 == n2);
    assert(!(n1 == n3));
    assert(n3 > n1);
    assert(n1 < n3);
    assert(!(n1 > n3));
    assert(!(n3 < n1));

    return "PASSED";
}

string test_is_zero()
{ // проверка числа на нуль
    assert(Natural(0).is_zero());
    assert(!Natural(1).is_zero());
    assert(!Natural(100).is_zero());

    return "PASSED";
}

string test_increment()
{ // инкремент натуральных чисел
    Natural n1(5);
    Natural expected(6);

    // Префиксный инкремент (++n)
    assert(++n1 == expected);
    assert(n1 == expected);

    // Постфиксный инкремент (n++)
    n1 = 5;
    n1++;
    assert(n1 == Natural(6));
    assert(n1 == expected);

    return "PASSED";
}

string text_add()
{ // сложение натуральных чисел
    assert(Natural(2) + Natural(2) == Natural(4));
    assert(Natural(15) + Natural(15) == Natural(30));
    assert(Natural(0) + Natural(2) == Natural(2));
    assert(Natural(12345) + Natural(11111) == Natural(23456));
    assert(Natural(999) + Natural(10) == Natural(1009));

    return "PASSED";
}

string test_subtract()
{ // вычитание из большего натураньного числа меньшее натуральное
    assert(Natural(2) - Natural(0) == Natural(2));
    assert(Natural(123) - Natural(23) == Natural(100));

    // Проверка на исключения
    try
    {
        Natural(0) - Natural(15);
        assert(false); // Если исключение не было поймано
    }
    catch (const invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }
    try
    {
        Natural(1) - Natural(15);
        assert(false); // Если исключение не было поймано
    }
    catch (const invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_multiply_digit()
{
    assert(Natural(22) * Digit(2) == Natural(44));
    assert(Natural(0) * Digit(2) == Natural(0));
    assert(Natural(12) * Digit(1) == Natural(12));

    return "PASSED";
}

string test_multiply_pow10()
{
    assert(Natural(22).mul_pow10(0) == Natural(22));
    assert(Natural(22).mul_pow10(1) == Natural(220));
    assert(Natural(0).mul_pow10(13) == Natural(0));
    assert(Natural(0).mul_pow10(0) == Natural(0));

    return "PASSED";
}

string test_multiply_nn()
{ // умножение натуральных чисел
    assert(Natural(22) * Natural(20) == Natural(440));
    assert(Natural(0) * Natural(200) == Natural(0));
    assert(Natural(121) * Natural(1) == Natural(121));
    assert(Natural(1234) * Natural(1235) == Natural(1523990));

    return "PASSED";
}

string test_sub_m_n()
{
    assert(Natural(22).sub_nmul(Natural(11), 2) == Natural(0));
    assert(Natural(13).sub_nmul(Natural(15), 0) == Natural(13));
    assert(Natural(13).sub_nmul(Natural(0), 1) == Natural(13));
    assert(Natural(13).sub_nmul(Natural(10), 1) == Natural(3));

    // Проверка на исключение
    try
    {
        Natural(10).sub_nmul(Natural(11), 2); // n * d = 22, больше, чем this = 10
        assert(false);
    }
    catch (const invalid_argument &ex)
    {
    }

    return "PASSED";
}

string test_div_nn_nk()
{
    // Тесты для корректных случаев
    assert(Natural(100).div_nmul(Natural(2), Natural(1)) == Natural(5));
    // 100 / (2 * 10^1) = 5
    return "PASSED";
}

string test_division()
{ // деление натуральных чисел (с округлением вниз, делитель не нуль)
    assert(Natural(10000) / Natural(200) == Natural(50));
    assert(Natural(12) / Natural(3) == Natural(4));
    assert(Natural(15) / Natural(4) == Natural(3));
    assert(Natural(0) / Natural(4) == Natural(0));
    assert(Natural(15) / Natural(1) == Natural(15));
    assert(Natural(15) / Natural(15) == Natural(1));

    try
    {
        Natural(10) / Natural(0);
        assert(false);
    }
    catch (const invalid_argument &ex)
    {
        // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_mod()
{ // остаток от деления натуральных чисел (делитель не нуль)
    assert(Natural(151515) % Natural(432) == Natural(315));
    assert(Natural(10) % Natural(2) == Natural(0));
    assert(Natural(13) % Natural(3) == Natural(1));
    assert(Natural(15) % Natural(4) == Natural(3));
    assert(Natural(1) % Natural(4) == Natural(1));
    assert(Natural(0) % Natural(4) == Natural(0));

    // Проверка на исключение
    // try {
    //   Natural(10) % Natural(0);
    //   assert(false);
    // } catch (const invalid_argument& ex) {
    //   // Исключение должно быть поймано
    // }

    return "PASSED";
}

string test_NOD()
{ // наибольший общий делитель
    assert(gcf(Natural(12), Natural(12)) == Natural(12));
    assert(gcf(Natural(1245), Natural(12)) == Natural(3));
    assert(gcf(Natural(55567), Natural(3456)) == Natural(1));
    assert(gcf(Natural(55), Natural(1)) == Natural(1));
    assert(gcf(Natural(55), Natural(0)) == Natural(55));

    return "PASSED";
}

string test_NOK()
{ // наименьшее общее кратное
    assert(lcm(Natural(12), Natural(12)) == Natural(12));
    assert(lcm(Natural(1234), Natural(124)) == Natural(76508));
    assert(lcm(Natural(55), Natural(1)) == Natural(55));
    assert(lcm(Natural(15), Natural(0)) == Natural(0));

    return "PASSED";
}

int main()
{
    try
    {
        cout << "Запуск тестов..." << endl;
        cout << "Тест компаратора натуральных чисел: " << test_compare() << endl;
        cout << "Тест проверки на нуль для натуральных чисел: " << test_is_zero() << endl;
        cout << "Тест инкремента натуральных чисел: " << test_increment() << endl;
        cout << "Тест сложения натуральных чисел: " << text_add() << endl;
        cout << "Тест вычитания натуральных чисел: " << test_subtract() << endl;
        cout << "Тест умножения на цифру для натуральных чисел: " << test_multiply_digit() << endl;
        cout << "Тест умножения на k-ую степень 10 для натуральных чисел: " << test_multiply_pow10() << endl;
        cout << "Тест умножения натуральных чисел: " << test_multiply_nn() << endl;
        cout << "Тест вычитания числа, умноженного на цифру(если результат положителен) для натуральных чисел: " << test_sub_m_n() << endl;
        cout << "Тест вычисления первой цифры деления для натуральных чисел: " << test_div_nn_nk() << endl;
        cout << "Тест деления натуральных чисел: " << test_division() << endl;
        cout << "Тест остатка от деления натуральных чисел: " << test_mod() << endl;
        cout << "Тест НОД: " << test_NOD() << endl;
        cout << "Тест НОК: " << test_NOK() << endl;
    }
    catch (const exception &ex)
    {
        cout << ex.what() << endl;
    }
    return 0;
}