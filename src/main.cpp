#include <iostream>
#include "Subject.hpp"
#include "Engine.hpp"

int main() {
    std::cout << "Lab work: Wrapper" << std::endl;

    Subject subj;
    Engine engine;

    std::cout << "Engine initialized" << std::endl;

    subj.f3(10, 5);

    return 0;
}