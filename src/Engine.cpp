#include "Engine.hpp"
#include <iostream>
#include <stdexcept>

void Engine::register_command(Wrapper* wrapper, const std::string& commandName) {
    if (wrapper == nullptr) {
        throw std::invalid_argument("Engine: Wrapper pointer cannot be null.");
    }

    if (commands.find(commandName) != commands.end()) {
        throw std::runtime_error("Engine: Command '" + commandName + "' already registered!");
    }

    commands[commandName] = wrapper;
    std::cout << "[Engine] New command registered: " << commandName << std::endl;
}

int Engine::execute(const std::string& commandName, const std::map<std::string, int>& args) {
    auto it = commands.find(commandName);

    if (it == commands.end()) {
        throw std::out_of_range("Engine: Command '" + commandName + "' is not registered.");
    }

    std::cout << "[Engine] Executing command '" << commandName << "'..." << std::endl;

    return it->second->execute(args);
}