#pragma once

#include "CalculatorTypes.h"
#include <string>
#include <vector>
#include <unordered_map>

class PluginLoader {
public:
    PluginLoader();
    ~PluginLoader();

    
    PluginLoader(const PluginLoader&) = delete;
    PluginLoader& operator=(const PluginLoader&) = delete;

    // Загружает плагины из указанной директории
    std::unordered_map<std::string, FunctionEntry> loadPlugins(const std::string& directoryPath);

private:
    std::vector<void*> plugin_handles;
};