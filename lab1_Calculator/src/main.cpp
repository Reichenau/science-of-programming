#include <iostream>
#include <locale.h>
#include "Calculator.h"

int main(){
    setlocale(LC_ALL, "ru");
    Calculator calc;

    calc.loadPlugins();

    std::string expression = "16 + 4 * ( 3 - 1 )";

    double result = calc.calculate(expression);

    std::cout << expression << std::endl;

    std::cout << "Результат: " << result;

    return 0;
}


