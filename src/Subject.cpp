#include "Subject.hpp"

int Subject::f3(int a, int b) {
    std::cout << "Subject::f3(" << a << ", " << b << ") called." << std::endl;
    return a - b; 
}