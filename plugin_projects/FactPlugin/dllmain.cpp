#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
    return "fact";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
    if (operand < 0 || operand != static_cast<int>(operand)) {
        throw std::runtime_error("Отрицательное значение");
    }
    double result = 1.0;
    for (int i = 2; i <= static_cast<int>(operand); ++i) {
        result *= i;
    }
    return result;
}