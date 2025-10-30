#pragma once

#include <string>
#include <vector>
#include <unordered_map>

using UnaryFunctionPtr = double(*)(double);
using BinaryFunctionPtr = double(*)(double, double);

class Calculator {
public:
	Calculator();
	~Calculator();

	void loadPlugins();
	double calculate(const std::string& expression);

private:
	enum class TokenType {
		NONE,       
		NUMBER,
		OPERATOR,
		FUNCTION,
		LEFT_PAREN,
		RIGHT_PAREN
	};

	std::vector<std::string> to_rpn(const std::string& expression);
	double evaluate_rpn(const std::vector<std::string>& rpn_tokens);
	int get_precedence(const std::string& op);
	bool is_operator(const std::string& token);
	bool try_parse_number(const std::string& token, double& value);

	struct FunctionEntry {
		size_t arity = 0;
		UnaryFunctionPtr unary = nullptr;
		BinaryFunctionPtr binary = nullptr;
	};

	std::unordered_map<std::string, FunctionEntry> functions;
	std::vector<void*> plugin_handles;
};