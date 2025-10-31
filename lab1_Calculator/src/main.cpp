#include <iostream>
#include <locale.h>
#include <string>
#include "Calculator.h"

int main() {
    setlocale(LC_ALL, "ru");

    Calculator calc;
    try {
        calc.loadPlugins();
    }
    catch (const std::exception& e) {
        std::cerr << "Ошибка при загрузке пагинов: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Консольный калькулятор. Введите 'exit' для выхода." << std::endl;

    std::string expression;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, expression);

        if (expression == "exit") {
            break;
        }
        if (expression.empty()) {
            continue;
        }

        try {
            double result = calc.calculate(expression);
            std::cout << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }
    return 0;
}