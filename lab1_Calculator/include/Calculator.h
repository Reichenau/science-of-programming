#pragma once

#include "CalculatorTypes.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <memory> 


class PluginLoader;
class ExpressionParser;

class Calculator {
public:
    Calculator();
    ~Calculator(); 

    void loadPlugins();
    double calculate(const std::string& expression);

private:
    std::unordered_map<std::string, FunctionEntry> functions;

    std::unique_ptr<PluginLoader> pluginLoader;
    std::unique_ptr<ExpressionParser> parser;
};