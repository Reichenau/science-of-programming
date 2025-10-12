#pragma once

#include <string>
#include <vector>

class Calculator {
public:
	double calculate(const std::string& expression);

private:
	std::vector<std::string> to_rpn(const std::string& expression);
	double evaluate_rpn(const std::vector<std::string>& rpn_tokens);
	int get_precedence(const std::string& op);
};