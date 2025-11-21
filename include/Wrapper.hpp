#pragma once
#include <map>
#include <string>

// Абстрактный базовый класс для всех оберток
class Wrapper {
public:
    virtual ~Wrapper() = default;

    virtual int execute(const std::map<std::string, int>& args) = 0;
};