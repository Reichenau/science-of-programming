#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
    return "ln";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
    if (operand <= 0) {
        throw std::runtime_error("Отрицательное значение");
    }
    return std::log(operand);
}