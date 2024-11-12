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
            this->data = from_string(str);
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