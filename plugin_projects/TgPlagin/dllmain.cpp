#include "pch.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
	return "tg";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
    if (static_cast<int>(operand) % 180 != 0 && static_cast<int>(operand) % 90 == 0) {
        throw std::runtime_error("Тангенс не определен для этого угла");
    }
    double radians = operand * M_PI / 180.0;
    return std::tan(radians);
}