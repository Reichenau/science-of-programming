#pragma once
#include "Wrapper.hpp"
#include <map>
#include <string>
#include <iostream>
#include <utility> 

// Шаблонный класс WrapperImpl
template<typename T, typename... Args>
class WrapperImpl : public Wrapper {
public:
    using MethodType = int (T::*)(Args...);

    WrapperImpl(T* subj, MethodType method, const std::map<std::string, int>& args)
        : subject(subj), method(method), arguments(args) {
        if (subject == nullptr) {
            throw std::invalid_argument("WrapperImpl: Subject pointer cannot be null.");
        }
    }

    int execute(const std::map<std::string, int>& inputArgs) override {
        return executeInternal(inputArgs, std::index_sequence_for<Args...>{});
    }

private:
    T* subject;                 
    MethodType method;          
    std::map<std::string, int> arguments; 

    template<size_t... Indices>
    int executeInternal(const std::map<std::string, int>& inputArgs, std::index_sequence<Indices...>) {
        return (subject->*method)(getArgumentValue(inputArgs, Indices)...);
    }

    // Получение значения аргумента по индексу
    int getArgumentValue(const std::map<std::string, int>& inputArgs, size_t index) {
        std::string key = "arg" + std::to_string(index + 1);

        // 1. Ищем в переданных аргументах 
        auto it = inputArgs.find(key);
        if (it != inputArgs.end()) {
            return it->second;
        }

        // 2. Ищем в аргументах по умолчанию
        it = arguments.find(key);
        if (it != arguments.end()) {
            return it->second;
        }

        // 3. Если не нашли — предупреждение 
        throw std::invalid_argument("WrapperImpl: Argument '" + key + "' not found provided.");
    }
};