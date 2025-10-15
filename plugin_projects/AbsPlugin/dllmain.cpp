#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
    return "abs";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
    return std::abs(operand);
}