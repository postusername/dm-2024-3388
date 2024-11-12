#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cassert>
#include <iostream>

namespace std { namespace rel_ops { struct make_rel_ops_work {}; } }
using namespace std;


class Digit: private rel_ops::make_rel_ops_work{
    public:
        Digit() : d(0), carry(0) {} 
        Digit(int d){
            this->d = d%10;
            this->carry = d/10; 
        }

        Digit(string str): Digit(str[0] - '0') {}

        int compare(Digit d) const{
            if (this->d > d.d) return 2;
            if (this->d < d.d) return 1;
            return 0;
        }

        friend const bool operator<(const Digit& lhs, const Digit& rhs){
            return lhs.compare(rhs) == 1;
        }

        friend const bool operator==(const Digit& lhs, const Digit& rhs){
            return lhs.compare(rhs) == 0;
        }

        friend const Digit operator+(const Digit& lhs, const Digit& rhs){
            return Digit(lhs.d + rhs.d);
        }

        friend const Digit operator-(const Digit& lhs, const Digit& rhs){
            if (lhs.d < rhs.d)
                throw invalid_argument("l < r");
            return Digit(lhs.d - rhs.d);
        }

        friend const Digit operator*(const Digit& lhs, const Digit& rhs){
            return Digit((int)(lhs.d * rhs.d));
        }

        friend const Digit operator/(const Digit& lhs, const Digit& rhs){
            return Digit(lhs.d / rhs.d);
        }

        friend ostream& operator<<(ostream& stream, const Digit& digit){
            stream<<digit.d;
            return stream;
        }
        int d;
        int carry;
};


template<typename T>
class NumberInterface{
    public:
        NumberInterface() {}
        NumberInterface(string str){
            this->data = new T[str.size()];
            for (auto i = 0; i < str.size(); ++i){
                this->data[i] = T(str[str.size() - i - 1]);
            }
        }

        virtual T from_string(string str) = 0;

        friend ostream& operator<<(ostream& stream, const NumberInterface& number){
            for (auto item: number.data)
                stream<<item;
            return stream;
        }
    protected:
        T data;
};


class Natural : public NumberInterface<vector<Digit>>, private rel_ops::make_rel_ops_work{
    public:
        Natural(string str){
            this->data = from_string(str);
        }
        Natural(uint64_t num): Natural(to_string(num)) {}

        friend ostream& operator<<(ostream& stream, const Natural& number){
            for (auto it = number.data.rbegin(); it != number.data.rend(); ++it)
                stream<<it->d;
            return stream;
        }

        vector<Digit> from_string(string str){
            vector<Digit> data;
            for (size_t i = 0; i < str.size(); ++i){
                data.push_back(Digit(str[i] - '0'));
                // cerr<<(int)data.at(i).d<<" ";
            }
            reverse(data.begin(), data.end());
            return data;
        }

        Digit& operator[](size_t i){
            return this->data.at(i);
        }

        int compare(Natural num) const{
            //cout<<*this<<"!@!"<<num<<endl;
            //cout<<this->data.size()<<"!@!"<<num.data.size()<<endl;
            if (this->data.size() > num.data.size())
                return 2;
            if (this->data.size() < num.data.size())
                return 1;
            for (long int i = this->data.size() - 1; i >= 0; --i){
                //cerr<<this->data.at(i)<<"  "<<num.data.at(i)<<endl;
                if (this->data.at(i) > num.data.at(i))
                    return 2;
                if (this->data.at(i) < num.data.at(i))
                    return 1;
            }
            return 0;
        }

        friend const bool operator<(const Natural& left, const Natural& right){
            return left.compare(right) == 1;
        }

        friend const bool operator==(const Natural& left, const Natural& right){
            return left.compare(right) == 0;
        }

        bool is_zero(){
            return this->data.at(this->data.size() - 1) == Digit(0);
        }

        Natural& operator++(){
            this->data.at(0) = this->data.at(0) + Digit(1);
            this->carry_flex(0);
            return *this;
        }

        Natural& operator++(int){
            Natural temp = *this;
            ++(*this);             
            return temp;           
        }

        void carry_flex(size_t i){
            if (this->data.at(i).carry != 0){
                for (size_t j = i + 1; j < this->data.size(); ++j){
                    this->data.at(j) = this->data.at(j) + this->data.at(j - 1).carry;
                    if (this->data.at(j).carry == 0)
                        return;
                    this->data.at(j - 1).carry = 0;
                }
                this->data.push_back(Digit(this->data.at(this->data.size() - 1).carry));
                this->data.at(this->data.size() - 1).carry = 0;
            }
        }

        void zero_flex(){
            size_t k = 0;
            for (size_t i = this->data.size() - 1; i > 0; --i){
                if (this->data.at(i) == Digit(0)) k++;
                else break;
            }
            for (size_t i = 0; i < k; ++i)
                this->data.pop_back();
        }

        friend const Natural operator+(const Natural& left, const Natural& right){
            Natural result = Natural(0);
            Natural r = Natural(0);
            if (left.data.size() < right.data.size()){
                result = right;
                r = left;
            }
            else{
                result = left;
                r = right;
            }
            for (size_t i = 0; i < r.data.size(); ++i){
                result.data.at(i) = result.data.at(i) + r.data.at(i);
                result.carry_flex(i);
            }
            result.zero_flex();
            return result;
        }

        friend const Natural operator-(const Natural& left, const Natural& right){
            //cout<<left<<" "<<right<<endl;
            if (left.compare(right) == 1)
                throw invalid_argument("l < r");
            //cout<<left<<" "<<right<<endl;
            Natural result = left;
            for (size_t i = 0; i < min(left.data.size(), right.data.size()); ++i){
                if (result.data.at(i) < right.data.at(i)){
                    for (size_t j = i + 1; j < result.data.size(); ++j){
                        if (result.data.at(j) > Digit(0)){
                            result.data.at(j) = result.data.at(j) - Digit(1);
                            break;
                        }
                    }
                }
                result.data.at(i) = result.data.at(i) - right.data.at(i);
            }
            result.zero_flex();
            return result;
        }

        friend const Natural operator*(const Natural& num, const Digit& digit){
            Natural result(0);
            for (size_t i = 0; i < digit.d; ++i){
                result = result + num;
            }
            return result;
        }

        friend const Natural operator*(const Digit& digit, const Natural& num){
            return num * digit;
        }

        Natural& mul_pow10(const Natural k){
            reverse(this->data.begin(), this->data.end());
            size_t tpi = 1;
            for (size_t i = 0; i < k.data.size(); ++i){
                for (size_t j = 0; j < k.data.at(i).d*tpi; ++j){
                    this->data.push_back(Digit(0));
                }
                tpi *= 10;
            }
            reverse(this->data.begin(), this->data.end());
            this->zero_flex();
            return *this;
        }

        friend const Natural operator*(const Natural& left, const Natural& right){
            Natural result = Natural(0);
            Natural tmp = Natural(0);
            Natural l = left;
            Natural r = right;
            for (size_t j = 0; j < right.data.size(); ++j){
                tmp = l * right.data.at(j);
                result = result + tmp.mul_pow10(Natural(j)); 
            }
            result.zero_flex();
            return result;
        }

        Natural& sub_nmul(const Natural& n, const Digit& d){
            Natural res = *this - (n * d);
            *this = res;
            return *this;
        }

        Natural& div_nmul(const Natural& n, const Natural k) {
            if (n == Natural(0))
                return *this;
            if (n > *this){
                *this = Natural(1);
                return *this;
            }
            Natural z(1);
            Natural tmp = n;
            tmp.mul_pow10(k);
            Natural n2 = tmp;
            while (tmp <= *this){
                z++;
                tmp = tmp + n2;
            }

            *this = Natural(z.data.at(z.data.size()-1).d) - 1;
            return *this; 
        }

        friend const Natural operator/(const Natural& left, const Natural& right) {
            if (right == Natural(0))
                throw invalid_argument("Zero division");
            if (left == Natural(0) || left < right)
                return Natural(0);
            if (left == right)
                return Natural(1);
            Natural z(0);
            Natural tmp = right;
            while (tmp <= left){
                z++;
                tmp = tmp + right;
            }
            return z; 
        }

        friend const Natural operator%(const Natural& left, const Natural& right){
            return left - (left / right)*right;;
        }

    protected:
        vector<Digit> data;
};


Natural gcf(const Natural& a, const Natural& b){
    Natural l = a;
    Natural r = b;
    while (l != Natural(0) && r != Natural(0)){
        if (l > r)
            l = l % r;
        else
            r = r % l;
    }
    return l + r;
}


Natural lcm(const Natural& a, const Natural& b){
    return a*b/gcf(a, b);
}


string test_compare() {                             //компаратор натуральных чисел (возвращаемое значение нестандартно!!)
    Natural n1(5), n2(5), n3(10);
    assert(n1 == n2);
    assert(!(n1 == n3));
    assert(n3 > n1);
    assert(n1 < n3);
    assert(!(n1 > n3));
    assert(!(n3 < n1));

    return "PASSED";
}

string test_is_zero() {                             //проверка числа на нуль
    assert(Natural(0).is_zero());
    assert(!Natural(1).is_zero());
    assert(!Natural(100).is_zero());
    

    return "PASSED";
}

string test_increment() {                              //инкремент натуральных чисел                       
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

string text_add() {                                //сложение натуральных чисел    
    assert(Natural(2)+ Natural(2) == Natural(4));
    assert(Natural(15)+ Natural(15) == Natural(30));
    assert(Natural(0)+ Natural(2) == Natural(2));
    assert(Natural(12345)+ Natural(11111) == Natural(23456));
    assert(Natural(999)+ Natural(10) == Natural(1009));

    return "PASSED";
}

string test_subtract() {                                //вычитание из большего натураньного числа меньшее натуральное    
    assert(Natural(2) - Natural(0) == Natural(2));
    assert(Natural(123)- Natural(23) == Natural(100));

    //Проверка на исключения
    try {
      Natural(0) - Natural(15);
      assert(false); // Если исключение не было поймано
    }   catch (const invalid_argument& ex) {
    // Исключение должно быть поймано
   }
    try {
      Natural(1) - Natural(15);
      assert(false); // Если исключение не было поймано
    }   catch (const invalid_argument& ex) {
    // Исключение должно быть поймано
    }

    return "PASSED";
}

                        

string test_multiply_digit() { 
    assert(Natural(22)* Natural(2) == Natural(44));
    assert(Natural(0)* Natural(2) == Natural(0));
    assert(Natural(12)* Natural(1) == Natural(12));

    return "PASSED";
}

string test_multiply_pow10() {   
    assert(Natural(22).mul_pow10(0) == Natural(22));
    assert(Natural(22).mul_pow10(1) == Natural(220));
    assert(Natural(0).mul_pow10(13) == Natural(0));
    assert(Natural(0).mul_pow10(0) == Natural(0));

    return "PASSED";
}

string test_multiply_nn() {                                //умножение натуральных чисел    
    assert(Natural(22)* Natural(20) == Natural(440));
    assert(Natural(0)* Natural(200) == Natural(0));
    assert(Natural(121)* Natural(1) == Natural(121));
    assert(Natural(1234)* Natural(1235) == Natural(1523990));

    return "PASSED";
}

string test_sub_m_n() {                
    cout<<111<<endl;                //вычитание числа, умноженного на цифру(если результат положителен)    
    assert(Natural(22).sub_nmul(Natural(11),2) == Natural(0));
    assert(Natural(13).sub_nmul(Natural(15),0) == Natural(13));
    assert(Natural(13).sub_nmul(Natural(0),1) == Natural(13));
    assert(Natural(13).sub_nmul(Natural(10),1) == Natural(3));

    // Проверка на исключение
    try {
      Natural(10).sub_nmul(Natural(11), 2); // n * d = 22, больше, чем this = 10
      assert(false); 
    } catch (const invalid_argument& ex) {
      cout<<11111111111<<endl;
    }

    return "PASSED";
}

string test_div_nn_nk() {
    // Тесты для корректных случаев
    cout<<Natural(100).div_nmul(Natural(2), Natural(1))<<endl; //Вычисление первой цифры деления большего натурального на меньшее, домноженное на 10^k, где k - номер позиции этой цифры (номер считается с нуля)
    assert(Natural(100).div_nmul(Natural(2), Natural(1)) == Natural(5)); 
    // 100 / (2 * 10^1) = 5
    return "PASSED";
}

string test_division() {                     //деление натуральных чисел (с округлением вниз, делитель не нуль)
    assert(Natural(10000) / Natural(200) == Natural(50));
    assert(Natural(12) / Natural(3) == Natural(4));
    assert(Natural(15) / Natural(4) == Natural(3)); 
    assert(Natural(0) / Natural(4) == Natural(0));
    assert(Natural(15) / Natural(1) == Natural(15));
    assert(Natural(15) / Natural(15) == Natural(1));

    try {
      Natural(10) / Natural(0); 
      assert(false); 
    } catch (const invalid_argument& ex) {
      // Исключение должно быть поймано
    }

    return "PASSED";
}

string test_mod() {                                          //остаток от деления натуральных чисел (делитель не нуль)
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

// int main() {
//     try{
//          cout << "Запуск тестов..." << endl;
//         cout << "Тест компаратора: " << test_compare() << endl;
//         cout << "Тест проверки на нуль: " << test_is_zero() << endl;
//         cout << "Тест инкремента: " << test_increment() << endl;
//         cout << "Тест сложения: " << text_add() << endl;
//         cout << "Тест вычитания: " << test_subtract() << endl;
//         cout << "Тест умножения на цифру: " << test_multiply_digit() << endl;
//         cout << "Тест умножения на k-ую степень 10: " << test_multiply_pow10() << endl;
//         cout << "Тест умножения натуральных чисел: " << test_multiply_nn() << endl;
//         cout << "Тест вычитания числа, умноженного на цифру(если результат положителен): " << test_sub_m_n() << endl;
//         cout << "Тест вычисления первой цифры деления: " << test_div_nn_nk() << endl;
//         cout << "Тест деления: " << test_division() << endl;
//         cout << "Тест остатка от деления: " << test_mod() << endl;
//     }catch(const exception& ex){
//         cout<<ex.what()<<endl;
//     }
//     return 0;
// }