#pragma once
#include "Wrapper.hpp"
#include <map>
#include <string>
#include <iostream>
#include <utility> 
#include <stdexcept>
#include <any>
#include <tuple>
#include <type_traits>
#include <vector>

// Шаблонный класс WrapperImpl
// T - тип класса
// R - тип возвращаемого значения 
template<typename T, typename R, typename... Args>
class Wrapper : public IWrapper {
public:
    using MethodType = R(T::*)(Args...);

    Wrapper(T* subj, MethodType method, const std::map<std::string, std::any>& args, const std::vector<std::string>& names = {})
        : subject(subj), method(method), arguments(args), argNames(names) {
        if (!subject) {
            throw std::runtime_error("Subject is null");
        }

        if (argNames.empty() && !arguments.empty()) {
            for (const auto& pair : arguments) {
                argNames.push_back(pair.first);
            }
        }
    }

    std::any execute(const std::map<std::string, std::any>& inputArgs) override {
        return executeInternal(inputArgs, std::index_sequence_for<Args...>{});
    }

private:
    T* subject;
    MethodType method;
    std::map<std::string, std::any> arguments;
    std::vector<std::string> argNames;

    using ArgsTuple = std::tuple<Args...>;

    template<size_t... Indices>
    std::any executeInternal(const std::map<std::string, std::any>& inputArgs, std::index_sequence<Indices...>) {
        if constexpr (std::is_void_v<R>) {
            (subject->*method)(getArgumentValue<Indices>(inputArgs)...);
            return {};
        }
        else {
            return (subject->*method)(getArgumentValue<Indices>(inputArgs)...);
        }
    }

    // Получение значения аргумента по индексу
    template<size_t index>
    auto getArgumentValue(const std::map<std::string, std::any>& inputArgs) {
        using ArgType = std::tuple_element_t<index, ArgsTuple>;

        std::string key;
        if (index < argNames.size()) {
            key = argNames[index];
        }
        else{
            key = "arg" + std::to_string(index + 1);
        }

        // 1. Ищем в переданных аргументах 
        auto it = inputArgs.find(key);
        if (it != inputArgs.end()) {
            return std::any_cast<ArgType>(it->second);
        }

        // 2. Ищем в аргументах по умолчанию
        it = arguments.find(key);
        if (it != arguments.end()) {
            return std::any_cast<ArgType>(it->second);
        }

        // 3. Если не нашли - предупреждение 
        throw std::invalid_argument("Missing argument");
    }
};