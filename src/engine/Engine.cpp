#include "engine/Engine.hpp" 
#include <iostream>
#include <stdexcept>

void Engine::register_command(std::unique_ptr<IWrapper> wrapper, const std::string& commandName) {
    if (!wrapper) {
        throw std::runtime_error("Wrapper is null");
    }

    if (commands.count(commandName)) {
        throw std::runtime_error("Command already exists: " + commandName);
    }

    commands[commandName] = std::move(wrapper);
}

std::any Engine::execute(const std::string& commandName, const std::map<std::string, std::any>& args) {
    auto it = commands.find(commandName);
    if (it == commands.end()) {
        throw std::runtime_error("Command not found: " + commandName);
    }
    return it->second->execute(args);
}