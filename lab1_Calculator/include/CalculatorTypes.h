#pragma once

#include <string>
#include <vector>
#include <unordered_map>

// Псевдонимы для указателей на функции
using UnaryFunctionPtr = double(*)(double);
using BinaryFunctionPtr = double(*)(double, double);

// Структура для хранения информации о функции
struct FunctionEntry {
    size_t arity = 0;
    UnaryFunctionPtr unary = nullptr;
    BinaryFunctionPtr binary = nullptr;
};

// Типы токенов для парсера
enum class TokenType {
    NONE,
    NUMBER,
    OPERATOR,
    FUNCTION,
    LEFT_PAREN,
    RIGHT_PAREN
};