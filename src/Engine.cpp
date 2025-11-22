#include "Engine.hpp"
#include <iostream>
#include <stdexcept>

void Engine::register_command(Wrapper* wrapper, const std::string& commandName) {
    if (commands.find(commandName) != commands.end()) {
        throw std::runtime_error("Error: command '" + commandName + "' already registered!");
    }

    commands[commandName] = wrapper;
    std::cout << "[Engine] New command registered: " << commandName << std::endl;
}

int Engine::execute(const std::string& commandName, const std::map<std::string, int>& args) {
    auto it = commands.find(commandName);

    if (it == commands.end()) {
        std::cerr << "[Engine] Error: Command '" << commandName << "' is not registered" << std::endl;
        return 0;
    }

    std::cout << "[Engine] Executing command '" << commandName << "'..." << std::endl;

    return it->second->execute(args);
}