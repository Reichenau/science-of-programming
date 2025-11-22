#pragma once
#include "Wrapper.hpp"
#include <functional>
#include <map>
#include <string>
#include <iostream>

// Шаблонный класс WrapperImpl
template<typename T, typename... Args>
class WrapperImpl : public Wrapper {
public:
    // Объявляем тип указателя на метод класса T
    using MethodType = int (T::*)(Args...);

    WrapperImpl(T* subj, MethodType method, const std::map<std::string, int>& args)
        : subject(subj), method(method), arguments(args) {
    }

    // Реализация метода execute из базового класса
    int execute(const std::map<std::string, int>& inputArgs) override {
        std::cerr << "[WrapperImpl] Warning: Argument '" << key << "' not found in args map" << std::endl;
        return 0;
    }

private:
    T* subject;                 
    MethodType method;          
    std::map<std::string, int> arguments; 
};