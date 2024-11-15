#include "polynomial.h"


std::map<Natural, Rational> Polynomial::from_string(std::string input) {
    std::map<Natural, Rational> data;
        std::regex full_regex(R"(^Poly\[((\[(Frac\[\s*-?\d+\s*,\s*[1-9]\d*\]|-?\d+)\s*,\s*-?[0-9]+\])(,\s*\[(Frac\[\s*-?\d+\s*,\s*[1-9]\d*\]|-?\d+)\s*,\s*[0-9]+\])*)\]$)");

        if (!std::regex_match(input, full_regex)) {
            throw std::invalid_argument("Invalid input format");
        }

        // Регулярное выражение для парсинга отдельных элементов внутри Poly
        std::regex term_regex(R"(\[((Frac\[\s*-?\d+\s*,\s*[1-9]\d*\])|(-?\d+))\s*,\s*([0-9]+)\])");

        // Извлекаем содержимое внутри Poly[...] для дальнейшего парсинга
        std::string terms = input.substr(5, input.size() - 6); // Убираем "Poly[" и "]" в начале и конце
        std::sregex_iterator terms_begin(terms.begin(), terms.end(), term_regex);
        std::sregex_iterator terms_end;
        Rational coefficient(Integer(0));
        data[Natural(0)] = coefficient; // инициализирум свободный член нулём 
        // Проверка совпадений и вывод результатов
        for (std::sregex_iterator i = terms_begin; i != terms_end; ++i) {
            std::smatch match = *i;
            Natural degree(match[4].str());

            if (match[2].matched) { // Если нашли Frac[a, b]
                coefficient = Rational(match[2].str());
            } else { // Если просто целое число
                coefficient = Rational(Integer(match[1].str()));
            }
            // для отладки
            if (!(degree > Natural(0) && coefficient == Rational(Integer(0)))) //если степень >0 и коэффициент 0, не создаем терм.
                data[degree] = coefficient;
    }

    return data;
}

std::string Polynomial::to_string() {
    std::string result = "Poly[";
    bool first = true;

    // Обратный обход terms для вывода от старших к младшим степеням
    for (auto it = this->data.rbegin(); it != this->data.rend(); ++it) {
        std::pair<Natural, Rational> term = *it;

        if (!first) {
            result += ", ";
        }
        first = false;

        if (term.second.get_denominator() == Integer(1))
            result += "[" + term.second.get_numerator().to_string() + ", " + term.first.to_string() + "]";
        else
            result += "[" + term.second.to_string() + ", " + term.first.to_string() + "]";
    }

    result += "]";
    return result;
}

Polynomial::Polynomial(const Rational num){
    //конструктор от рационального числа - создает многочлен степени 0
    this->data[Natural(0)] = num; 
}

Polynomial::Polynomial(std::initializer_list<std::pair<Natural, Rational>> init_list): Polynomial(){
    for (const auto& monome: init_list){
        this->data[monome.first] = monome.second;
    }
}

Polynomial::Polynomial(std::string str){
    this->data = from_string(str);
}

/* ADD_PP_P Лутфулин Д. А. 3388 */
Polynomial& Polynomial::operator+=(const Polynomial& other){
    //проходимся по мономам добавляемого многочлена и складываем соответствующие коэффициенты
    for (const auto& [deg, val] : other.data){
        this->data[deg] = this->data[deg] + val;
        //если выходит что текущий коэф. стал нулём - удаляем его
        if (deg > Natural(0) && (this->data[deg] == Rational(0) || this->data[deg] == Rational({Integer("-0"),Natural(1)})))
            this->data.erase(deg); 
    }
    return *this;
}


Polynomial& Polynomial::operator+=(const Rational& num){
    this->data[Natural(0)] = this->data[Natural(0)] + num;
    return *this;
}

/* SUB_PP_P Лутфулин Д. А. 3388 */
Polynomial& Polynomial::operator-=(const Polynomial& other){
    for (const auto& [deg, val] : other.data){
        this->data[deg] = this->data[deg] - val;
        //если получили коэффициент 0, то удаляем его, если это не нулевая степень(чтобы был многочлен "0")
        // if (deg > Natural(0) && this->data[deg] == Rational(0)) - ВЕРНУТЬ ВМЕСТО СЛЕДУЮЩЕЙ СТРОКИ ПОСЛЕ ИСПРАВЛЕНИЯ -0 != 0!!!!1!!!1!11!!!!!!!!!
        if (deg > Natural(0) && (this->data[deg] == Rational(0) || this->data[deg] == Rational({Integer("-0"),Natural(1)})))
            this->data.erase(deg); 
    }
    return *this;
}

Polynomial& Polynomial::operator-=(const Rational& num){
    this->data[0] = this->data[0] - num;
    return *this;
}

const Polynomial operator+(const Polynomial& left, const Polynomial& right){
    Polynomial result = left;
    result += right;
    return result;
}

const Polynomial operator+(const Polynomial& poly, const Rational& num){
    Polynomial result = poly;
    result += num;
    return result;
}

const Polynomial operator-(const Polynomial& left, const Polynomial& right){
    Polynomial result = left;
    result -= right;
    return result;
}

const Polynomial operator-(const Polynomial& poly, const Rational& num){
    Polynomial result = poly;
    result -= num;
    return result;
}

const bool operator==(const Polynomial& left,const Polynomial& right){
    Polynomial divv = left - right;
    // многочлены равны если их разность равна 0
    return (divv.degree() == Natural(0)) && (divv.lead() == Rational("Frac[-0,1]")) || (divv.lead() == Rational(0));
}

/* MUL_PQ_P Лутфулин Д. А. 3388 */
Polynomial& Polynomial::operator*=(const Rational& num){
    //проходимся по всем коэффциентам многочлена и умножаем на num
    if (num == Rational(0)){ // умножение на 0 дает многочлен равный 0 {
        this->data.clear();  // Очищаем все коэффициенты (полином становится нулевым)
        this->data[0] = Rational(0);  // Явно ставим коэффициент при нулевой степени равным 0
    }
    else{ // умножение полинома на 0 дает 0
        for (auto& [deg, val] : this->data){
            this->data[deg] = this->data[deg] * num;
        }
    }
    return *this;
}

const Polynomial operator*(const Polynomial& poly, const Rational& num){
    Polynomial new_poly = poly;
    new_poly *= num;
    return new_poly;
}

const Polynomial operator*(const Rational& num, const Polynomial& poly){
    return poly * num;
}

/* MUL_Pxk_P Лутфулин Д. А. 3388 */
Polynomial& Polynomial::mul_powX(const Natural k){
    std::map<Natural, Rational> new_data; //создаем новую data
    //проходимся по всем мономам вставляем коэффициенты степени deg старой data в степень deg+k новой
    for (auto& [deg, val] : this->data) {
        new_data[deg + k] = val;
    }
    this->data = std::move(new_data); // помещаем new_data в старую data
    return *this;
}

/* LED_P_Q Лутфулин Д. А. 3388 */
Rational Polynomial::lead() const{
    //коэффициент при старшей степени (старшая степень в правом конце map)
    return this->data.rbegin()->second;
}

/* DEG_P_N Лутфулин Д. А. 3388 */
Natural Polynomial::degree() const {
    //старшая степень в правом конце map
    return this->data.rbegin()->first;
} 

/* FAC_P_Q Лутфулин Д. А. 3388 */
Rational Polynomial::canonical(){
    Natural gcf_numerators = this->data.begin()->second.get_numerator().abs(); // начинаем собирать модули числителей для НОД с монома минимальной степени
    Natural lcm_denominators = this->data.begin()->second.get_denominator(); // начинаем собирать знаменатели для НОК с монома минимальной степени
    for(auto& [deg, val] : this->data){
        gcf_numerators = gcf(gcf_numerators, val.get_numerator().abs());
        lcm_denominators = lcm(lcm_denominators, val.get_denominator());
    }
    //ответ - НОД числителей / НОК знаменателей
    Rational ans = Rational(Integer(gcf_numerators), lcm_denominators);
    (*this) *= ans; // умножаем наш многочлен на получившуюся дробь
    return ans;
}

/*MUL_PP_P Лутфулин Д. А. 3388 */
const Polynomial operator*(const Polynomial& left, const Polynomial& right){
    Polynomial ans = Polynomial({{Natural(0),Rational(0)}}); // инициализируем ответ нулем
    Polynomial cur_term; //ответ - сумма мономов второго, умноженных на первый многочлен
    for(auto& [deg, val] : right.data){ //идем по мономам правого многочлена и умножаем их на левый
        cur_term = left * val; // cur_term = левый * коэф. текущего монома
        cur_term.mul_powX(deg); //cur_term = левый * текущий моном
        ans += cur_term;
    }
    return ans;
}

/*DIV_PP_P Лутфулин Д. А. 3388 */
const Polynomial operator/(const Polynomial& left,const Polynomial& right){
    Polynomial divident = left, quotient = Polynomial(Rational(0)), sub_coefficient; // делимое, делитель, вычитаемое из делимого
    Natural sub_lead(0); // степень вычитаемого коэфициента
    while (divident.degree() >= right.degree()){ // вычитаем из делимого sub_coefficient пока делимое не станет остатком
        sub_lead = divident.degree() - right.degree(); //вычисляем степень текущего одночлена 
        sub_coefficient = Polynomial(divident.lead() / right.lead()).mul_powX(sub_lead); // вычисление следующего 
        quotient += sub_coefficient; // прибавляем получившийся моном к частному
        divident -= sub_coefficient * right; // вычитаем произведение монома из делимого
    }
    return quotient;
}
/*MOD_PP_P Лутфулин Д. А. 3388 */
const Polynomial operator%(const Polynomial& left, const Polynomial& right){
    //вычисляем неполное частное
    Polynomial incomplete_quotient = (left / right) * right;
    // остаток = делимое - неполное частное
    return left - incomplete_quotient; 
}


/*GCF_PP_P Лутфулин Д. А. 3388 */
Polynomial gcf(Polynomial a, Polynomial b){
    //стандартный алгоритм НОД но для многочленов сравниваются степени
    while (a.degree() > Natural(0) && b.degree() > Natural(0)){
        if (a.degree() > b.degree())
            a = a % b;
        else
            b = b % a;
    }
    //возвращаем наибольшее по степени из a, b
    return (a.degree() > b.degree()) ? a : b;
}
/*DER_P_P Лутфулин Д. А. 3388 */
Polynomial derivative(const Polynomial& X){
    Polynomial result;
    for (const auto& [deg, val] : X.data) {
        if (deg > Natural(0)) {  // Пропускаем константный член (степень 0)
            result.data[deg - Natural(1)] = val * Rational(deg);  // Умножаем коэффициент на степень и уменьшаем степень на 1
        }
    }
    return result;
}
/*NMR_P_P Лутфулин Д. А. 3388 */
Polynomial normalize(const Polynomial& poly){
    //Если r - корень кратности k многочлена poly, то он кратности k-1 у производной. 
    //НОД производной и самого многочлена - произведение всех "лишних" кратных корней poly
    //Значит, если poly разделить на НОД(poly, poly') то останутся только его корни кратности 1  
    return poly / gcf(poly, derivative(poly));
}
