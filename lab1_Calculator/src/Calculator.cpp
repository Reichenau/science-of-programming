#include "Calculator.h"
#include "PluginLoader.h"     
#include "ExpressionParser.h" 
#include <iostream>

Calculator::Calculator()
    : pluginLoader(std::make_unique<PluginLoader>()),
    parser(std::make_unique<ExpressionParser>())
{
}

Calculator::~Calculator() = default;

void Calculator::loadPlugins() {
    try {
        functions = pluginLoader->loadPlugins("./plugins");
    }
    catch (const std::exception& e) {
        std::cerr << " Ошибка при загрузке плагинов: " << e.what() << std::endl;
    }
}

double Calculator::calculate(const std::string& expression) {
    return parser->calculate(expression, functions);
}