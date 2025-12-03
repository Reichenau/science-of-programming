#pragma once
#include <map>
#include <string>
#include <any>
#include <memory>
#include "wrapper/Wrapper.hpp"

// регистрация команды
class Engine {
public:
    // commandName - имя, по которому будем вызывать команду
    void register_command(IWrapper* wrapper, const std::string& commandName);

    // Выолнение команды 
    std::any execute(const std::string& commandName, const std::map<std::string, std::any>& args);

private:
    std::map<std::string, IWrapper*> commands;
};