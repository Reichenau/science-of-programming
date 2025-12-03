#pragma once
#include <map>
#include <string>
#include <any>

// Абстрактный базовый класс для всех оберток
class IWrapper {
public:
    virtual ~IWrapper() = default;

    virtual std::any execute(const std::map<std::string, std::any>& args) = 0;
};