#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using FunctionPtr = double(*)(double);

class Calculator {
public:
	Calculator();
	~Calculator();

	void loadPlugins();
	double calculate(const std::string& expression);

private:
	std::vector<std::string> to_rpn(const std::string& expression);
	double evaluate_rpn(const std::vector<std::string>& rpn_tokens);
	int get_precedence(const std::string& op);

	std::unordered_map<std::string, FunctionPtr> functions;

	std::vector<void*> plugin_handles;
};