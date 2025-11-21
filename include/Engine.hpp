#pragma once
#include <map>
#include <string>
#include "Wrapper.hpp"

// Класс движка, который управляет регистрацией и исполнением команд.
class Engine {
public:
    // commandName - имя, по которому будем вызывать команду
    void register_command(Wrapper* wrapper, const std::string& commandName);

    // Выполнение команды по имени с передачей аргументов
    int execute(const std::string& commandName, const std::map<std::string, int>& args);

private:
    std::map<std::string, Wrapper*> commands;
};