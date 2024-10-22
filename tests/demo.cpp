#include <cassert>
#include <iostream>
#include "../src/function.cpp"


std::string test_greetings() {
    assert(greetings("Bob") == "Hello, Bob!");
    assert(greetings("world") == "Hello, world!");

    return "PASSED";
}

int main() {
    std::cout << "TEST 1: " << test_greetings() << std::endl;
    return 0;
}
