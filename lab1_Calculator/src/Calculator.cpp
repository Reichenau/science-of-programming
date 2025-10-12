#include "Calculator.h"
#include <stack>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

double Calculator::calculate(const std::string& expression) {
	std::vector<std::string> rpn = to_rpn(expression);
	return evaluate_rpn(rpn);
}

// Реализация парсера 
std::vector<std::string> Calculator::to_rpn(const std::string& expression) {
	std::vector<std::string> output_queue;
	std::stack<std::string> operator_stack;

	std::stringstream ss(expression);
	std::string token;

	while (ss >> token) {
		// Проверка на то, что токен число
		if (isdigit(token[0]) || (token.length() > 1 && isdigit(token[1]))) {
			output_queue.push_back(token);
		}

		// Проверка на то, что токен один из оперторов
		else if (token == "+" || token == "-" || token == "*" || token == "/") {
			while (!operator_stack.empty() && get_precedence(operator_stack.top()) >= get_precedence(token)) {
				output_queue.push_back(operator_stack.top());
				operator_stack.pop();
			}
			operator_stack.push(token);
		}

		// Проверка на то, что токен ооткрывающая скобка
		else if (token == "(") {
			operator_stack.push(token);
		}

		// Проверка на то, что токен закрывающая скобка
		else if (token == ")") {
			// Перемещаем операторы из стека в очередь, пока не встретим открывающую скобку
			while (!operator_stack.empty() && operator_stack.top() != "(") {
				output_queue.push_back(operator_stack.top());
				operator_stack.pop();
			}
			// Удаляем открывающую скобку из стека
			if (!operator_stack.empty()) {
				operator_stack.pop();
			}
		}
	}
	// Перемещаем все оставшеся операторы из стека в очередь
	while (!operator_stack.empty()) {
		output_queue.push_back(operator_stack.top());
		operator_stack.pop();
	}

	return output_queue;
}

// Реализация вычисления 
double Calculator::evaluate_rpn(const std::vector<std::string>& rpn_tokens) {
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

// Функция для определения приоритета оператора 
int Calculator::get_precedence(const std::string& op) {
	static const std::unordered_map<std::string, int> precedence = {
		{"+", 1}, {"-", 1},
		{"*", 2}, {"/", 2}
	};

	auto it = precedence.find(op);
	return (it != precedence.end()) ? it->second : 0;
}