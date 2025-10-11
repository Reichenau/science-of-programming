#include "evaluator.h"
#include <stack>
#include <stdexcept>

// Функия для вычисления выражения из польской нотации  
double evaluate_rpn(const std::vector<std::string>& rpn_tokens) {
	std::stack<double> value_stack; // Стэк для чисел

	for (const std::string& token : rpn_tokens) {
		// Токен - число
		if (isdigit(token[0]) || (token.length() > 1 && isdigit(token[1]))) {
			value_stack.push(std::stod(token)); 
		}
		
		// Токен - оператор
		else {
			if (value_stack.size() < 2) {
				throw std::runtime_error("Ошибка: нет достаточного числа операндов.");
			}
			double operand2 = value_stack.top();
			value_stack.pop();

			double operand1 = value_stack.top();
			value_stack.pop();

			// Выполнение операций 
			if (token == "+") {
				value_stack.push(operand1 + operand2);
			}
			else if (token == "-") {
				value_stack.push(operand1 + operand2);
			}
			else if (token == "*") {
				value_stack.push(operand1 + operand2);
			}
			else if (token == "/") {
				if (operand2 == 0) {
					throw std::runtime_error("Ошибка: деление на 0.");
				}
				value_stack.push(operand1 + operand2);
			}
		}
	}

	if (value_stack.size() != 1) {
		throw std::runtime_error("Ошибка: слишком много операндов.");
	}

	return value_stack.top(); // Результат вычислений 
}