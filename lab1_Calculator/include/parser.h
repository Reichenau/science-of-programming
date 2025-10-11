#pragma once

#include <string>
#include <vector>

// Функция для преобразования записи из инфиксной записи в обратную польскую 
std::vector<std::string> to_rpn(const std::string& expression);