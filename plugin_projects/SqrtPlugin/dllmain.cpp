#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
    return "sqrt";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
    // Проверка, что подкоренное значение положительно  
    if (operand < 0) {
        throw std::runtime_error("Отрицательное подкоренное значение");
    }
    return std::sqrt(operand);
}