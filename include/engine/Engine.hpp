#pragma once
#include <map>
#include <string>
#include <any>
#include <memory>
#include "wrapper/Wrapper.hpp"

// Класс движка, который управляет регистрацией и исполнением команд.
class Engine {
public:
    // commandName - имя, по которому будем вызывать команду
    void register_command(std::unique_ptr<Wrapper> wrapper, const std::string& commandName);

    // Выполнение команды по имени с передачей аргументов
    std::any execute(const std::string& commandName, const std::map<std::string, std::any>& args);

private:
    std::map<std::string, std::unique_ptr<Wrapper>> commands;
};