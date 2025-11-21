#include <iostream>
#include "Subject.hpp"

int main() {
    std::cout << "Lab 2: Wrapper initialized" << std::endl;

    Subject subj;
    int result = subj.f3(10, 7);

    std::cout << "Result: " << result << std::endl;

    return 0;
}