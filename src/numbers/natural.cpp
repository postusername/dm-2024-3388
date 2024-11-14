#include "natural.h"


ostream &operator<<(ostream &stream, Natural &num)
{
    stream << num.to_string();
    return stream;
}


vector<Digit> Natural::from_string(string str) {
    std::vector<Digit> data;
    for (size_t i = 0; i < str.size(); ++i)
    {
        if (str[i] < '0' || str[i] > '9')
            throw std::invalid_argument("Invalid digit");
        data.push_back(Digit(str[i] - '0'));
    }
    reverse(data.begin(), data.end());
    return data;
}


string Natural::to_string(uint64_t num)
{
    return std::to_string(num);
}


string Natural::to_string(vector<Digit> num)
{
    string str = "";
    for (std::vector<Digit>::reverse_iterator it = num.rbegin(); it != num.rend(); ++it)
        str += std::to_string(it->d);
    return str;
}


string Natural::to_string()
{
    return to_string(this->data);
}


// COM_NN_D Беннер В. А. 3388
int Natural::compare(Natural num) const
{
    if (this->data.size() > num.data.size())
        return 2;
    if (this->data.size() < num.data.size())
        return 1;
    for (long int i = this->data.size() - 1; i >= 0; --i)
    {
        if (this->data.at(i) > num.data.at(i))
            return 2;
        if (this->data.at(i) < num.data.at(i))
            return 1;
    }
    return 0;
}


const bool operator<(const Natural &left, const Natural &right)
{
    return left.compare(right) == 1;
}


const bool operator==(const Natural &left, const Natural &right)
{
    return left.compare(right) == 0;
}


void Natural::carry_flex(size_t i)
{
    if (this->data.at(i).carry == 0)
        return; // Все ок, завершаем
    for (size_t j = i + 1; j < this->data.size(); ++j)
    {
        this->data.at(j) = this->data.at(j) + this->data.at(j - 1).carry; // Добавляем к следующему
        if (this->data.at(j).carry == 0)                                  // Если влезко, все ок, завершаем
            return;
        this->data.at(j - 1).carry = 0; // Обнуляем у предыдущего
    }
    this->data.push_back(Digit(this->data.at(this->data.size() - 1).carry)); // Если не влезло в текущие разряды, добавляем новый
    this->data.at(this->data.size() - 1).carry = 0;                          // Обнуляем у предыдущего
}


void Natural::zero_flex()
{
    size_t j = 0;
    for (size_t i = this->data.size() - 1; i > 0; --i)
    {
        if (this->data.at(i) == Digit(0))
            j++;
        else
            break; // Берем все незначащие нули
    }
    for (size_t i = 0; i < j; ++i)
        this->data.pop_back(); // Удаляем их
}


// NZER_N_B Беннер В. А. 3388
bool Natural::is_zero()
{
    return *this == Natural(0);
}


// ADD_1N_N Беннер В. А. 3388
Natural Natural::operator++()
{
    this->data.at(0) = this->data.at(0) + Digit(1);
    this->carry_flex(0);
    return *this;
}


Natural Natural::operator++(int)
{
    Natural old = *this;
    ++(*this);
    return old;
}


// ADD_NN_N Беннер В. А. 3388
const Natural operator+(const Natural &left, const Natural &right)
{
    Natural l(0), r(0);
    if (left.data.size() < right.data.size())
    {
        l = right;
        r = left;
    }
    else
    {
        l = left;
        r = right;
    } // Прибавляем к большему
    for (size_t i = 0; i < r.data.size(); ++i)
    {
        l.data.at(i) = l.data.at(i) + r.data.at(i); // Суммируем разряды
        l.carry_flex(i);                            // Переносим разряды
    }
    l.zero_flex(); // Убираем не значащие нули
    return l;
}


// SUB_NN_N Беннер В. А. 3388
const Natural operator-(const Natural &left, const Natural &right)
{
    if (left < right)
        throw invalid_argument("left value must be greater or equal of right value");
    Natural l = left;
    for (size_t i = 0; i < min(left.data.size(), right.data.size()); ++i)
    {
        if (l.data.at(i) < right.data.at(i))
        {
            for (size_t j = i + 1; j < l.data.size(); ++j)
            { // Ищем не нулевой разряд (всегда найдется, так как l > r)
                if (l.data.at(j) > Digit(0))
                {
                    l.data.at(j) = l.data.at(j) - Digit(1); // Заняли
                    break;
                }
                l.data.at(j) = Digit(9); // Попутным нулям + 9
            }
            l.data.at(i) = l.data.at(i) + Digit(1) + (Digit(9) - right.data.at(i)); // Добавили 10 и вычли разряды
        }
        else
        {
            l.data.at(i) = l.data.at(i) - right.data.at(i); // Все хорошо, просто вычли разряды
        }
    }
    l.zero_flex();
    return l;
}


// MUL_ND_N Беннер В. А. 3388
const Natural operator*(const Natural &num, const Digit &digit)
{
    Natural result(0);
    for (size_t i = 0; i < (size_t)digit.d; ++i)
        result = result + num;
    return result;
}


const Natural operator*(const Digit &digit, const Natural &num)
{
    return num * digit;
}


// MUL_Nk_N Беннер В. А. 3388
Natural &Natural::mul_pow10(const Natural k)
{
    reverse(this->data.begin(), this->data.end());
    size_t multiplier = 1;
    for (size_t i = 0; i < k.data.size(); ++i)
    {
        for (size_t j = 0; j < k.data.at(i).d * multiplier; ++j)
        {
            this->data.push_back(Digit(0)); // Добавляем нули в конец
        }
        multiplier *= 10;
    }
    reverse(this->data.begin(), this->data.end());
    this->zero_flex();
    return *this;
}


// MUL_NN_N Беннер В. А. 3388
const Natural operator*(const Natural &left, const Natural &right)
{
    Natural result(0), tmp(0), l = left;
    for (size_t j = 0; j < right.data.size(); ++j)
    {
        tmp = l * right.data.at(j);                  // Умножаем в столбик
        result = result + tmp.mul_pow10(Natural(j)); // Домножили на 10^j (как сдвиг в столбике)
    }
    result.zero_flex(); // Убрали лишнии нули
    return result;
}


// SUB_NDN_N Беннер В. А. 3388
Natural &Natural::sub_nmul(const Natural &n, const Digit &d)
{
    Natural res = *this - (n * d);
    *this = res;
    return *this;
}


// DIV_NN_Dk Беннер В. А. 3388
Natural &Natural::div_nmul(const Natural &n, const Natural k)
{
    if (n == Natural(0))
        throw invalid_argument("Zero division error");
    Natural i(0), tmp = n;
    tmp.mul_pow10(k); // Умножили делитель на 10^k
    Natural num = tmp;
    while (tmp <= *this)
    {
        i++;
        tmp = tmp + num;
    } // Посмотрели сколько раз помещается
    *this = Natural(i.data.at(i.data.size() - 1).d); // Вернули первую цифру
    return *this;
}


// DIV_NN_N Беннер В. А. 3388
const Natural operator/(const Natural &left, const Natural &right)
{
    if (right == Natural(0))
        throw invalid_argument("Zero division");
    Natural result(0), current_div_res(0), tmp = left;
    for (long int shift = left.data.size() - right.data.size(); shift >= 0; --shift)
    { // Делим на right^k, где k = shift
        current_div_res = tmp;
        current_div_res.div_nmul(right, shift).mul_pow10(shift); // Получили первую цифры, домоножили на сдвиг
        tmp = tmp - current_div_res * right;                     // Вычли (как при делении в столбик)
        result = result + current_div_res;                       // Зафиксировали текущий результат
    }
    return result;
}


// MOD_NN_N Беннер В. А. 3388
const Natural operator%(const Natural &left, const Natural &right)
{
    return left - (left / right) * right;
}


// GCF_NN_N Беннер В. А. 3388
Natural gcf(const Natural &a, const Natural &b)
{
    Natural l = a, r = b;
    while (l != Natural(0) && r != Natural(0))
    {
        if (l > r)
            l = l % r;
        else
            r = r % l;
    }
    return l + r;
}


// LCM_NN_N Беннер В. А. 3388
Natural lcm(const Natural &a, const Natural &b)
{
    return (a * b) / gcf(a, b);
}
