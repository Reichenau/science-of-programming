#pragma once
#include <map>
#include <string>
#include <any>

// Абстрактный базовый класс для всех оберток
class Wrapper {
public:
    virtual ~Wrapper() = default;

    virtual std::any execute(const std::map<std::string, std::any>& args) = 0;
};