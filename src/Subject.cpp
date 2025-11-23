#include "Subject.hpp"
#include <iostream>

int Subject::f3(int a, int b) {
    std::cout << "Subject::f3(" << a << ", " << b << ") called." << std::endl;
    return a - b;
}

void Subject::print(const std::string& message) {
    std::cout << "Subject::print(\"" << message << "\") called." << std::endl;
}