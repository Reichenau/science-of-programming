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

// Шаблонный класс WrapperImpl
// T - тип класса
// R - тип возарвщаемого значения 
template<typename T, typename R, typename... Args>
class WrapperImpl : public Wrapper {
public:
    using MethodType = R(T::*)(Args...);

    WrapperImpl(T* subj, MethodType method, const std::map<std::string, std::any>& args)
        : subject(subj), method(method), arguments(args) {
        if (subject == nullptr) {
            throw std::invalid_argument("WrapperImpl: Subject pointer cannot be null.");
        }
    }

    std::any execute(const std::map<std::string, std::any>& inputArgs) override {
        return executeInternal(inputArgs, std::index_sequence_for<Args...>{});
    }

private:
    T* subject;
    MethodType method;
    std::map<std::string, std::any> arguments;
    
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
        std::string key = "arg" + std::to_string(index + 1);

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

        // 3. Если не нашли — предупреждение 
        throw std::invalid_argument("WrapperImpl: Argument '" + key + "' not found provided.");
    }
};

template<typename T, typename R, typename... Args>
std::unique_ptr<Wrapper> make_wrapper(T* subj, R(T::* method)(Args...), const std::map<std::string, std::any>& args) {
    return std::make_unique<WrapperImpl<T, R, Args...>>(subj, method, args);
}