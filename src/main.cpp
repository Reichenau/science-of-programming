#include <iostream>
#include "Subject.hpp"
#include "Engine.hpp"

int main() {
    setlocale(LC_ALL, "ru");

    std::cout << "Лабораторная работа: Wrapper" << std::endl;

    Subject subj;
    Engine engine;

    std::cout << "Движок инициализирован" << std::endl;

    subj.f3(10, 5);

    return 0;
}