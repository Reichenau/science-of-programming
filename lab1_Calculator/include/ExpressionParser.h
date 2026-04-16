#pragma once

#include "CalculatorTypes.h"
#include <string>
#include <vector>
#include <unordered_map>

class ExpressionParser {
public:
    ExpressionParser() = default;

    // Главный метод
    double calculate(const std::string& expression,
        const std::unordered_map<std::string, FunctionEntry>& functions);

private:
    // Вспомогательные методы
    std::vector<std::string> to_rpn(const std::string& expression,
        const std::unordered_map<std::string, FunctionEntry>& functions);

    double evaluate_rpn(const std::vector<std::string>& rpn_tokens,
        const std::unordered_map<std::string, FunctionEntry>& functions);

    int get_precedence(const std::string& op,
        const std::unordered_map<std::string, FunctionEntry>& functions);

    bool is_operator(const std::string& token);
    bool try_parse_number(const std::string& token, double& value);
};