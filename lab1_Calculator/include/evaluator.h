#pragma once

#include <string>
#include <vector>

// Функия для вычисления выражения из польской нотации  
double evaluate_rpn(const std::vector<std::string>& rpn_tokens);