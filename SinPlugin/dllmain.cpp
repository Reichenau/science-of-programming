#include "pch.h"

#define _USE_MATH_DEFINES
#include <cmath>

#define DllExport extern "C" __declspec(dllexport)

// Возврат имени операции 
DllExport const char* getFunctionName() {
	return "sin";
}

// Функция, которая выполняет вычисления 
DllExport double execute(double operand) {
	double radians = operand * M_PI / 180.0;
	return std::sin(radians);
}