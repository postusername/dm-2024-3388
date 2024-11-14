#include <iostream>
#include "parser.h"


int main() {
    Parser parser;
    for (auto v : parser.tokenize("a = COM_NN_D(2,4)")) {
        std::cout << v.value << std::endl;
        std::cout << static_cast<int>(v.type) << std::endl;
    }
}
