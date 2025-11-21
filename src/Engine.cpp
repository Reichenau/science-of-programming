#include "Engine.hpp"
#include <iostream>
#include <stdexcept>

void Engine::register_command(Wrapper* wrapper, const std::string& commandName) {
    if (commands.find(commandName) != commands.end()) {
        throw std::runtime_error("Ошибка: команда '" + commandName + "' уже зарегистрирована!");
    }

    commands[commandName] = wrapper;
    std::cout << "[Engine] Зарегистрирована новая команда: " << commandName << std::endl;
}

int Engine::execute(const std::string& commandName, const std::map<std::string, int>& args) {
    auto it = commands.find(commandName);

    if (it == commands.end()) {
        std::cerr << "[Engine] Ошибка: команда '" << commandName << "' не найдена." << std::endl;
        return 0; 
    }

    std::cout << "[Engine] Вызов команды '" << commandName << "'..." << std::endl;

    return it->second->execute(args);
}