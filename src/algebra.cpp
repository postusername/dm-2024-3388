#include <iostream>
#include "parser.h"


int main() {
    Parser parser;
    std::string s;
    std::getline(std::cin, s);
    for (auto v : parser.tokenize(s)) {
        std::cout << v.value << std::endl;
        std::cout << static_cast<int>(v.type) << std::endl;
    }
}
