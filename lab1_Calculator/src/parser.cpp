#include "parser.h"
#include <stack>
#include <sstream>
#include <unordered_map>

// Функция для определения приоритета оператора 
int get_precedence(const std::string& op) {
	static const std::unordered_map<std::string, int> precedence = {
		{"+", 1}, {"-", 1},
		{"*", 2}, {"/", 2}
	};

	auto it = precedence.find(op);
	if (it != precedence.end()) {
		return it->second;
	}

	return 0;
}

// Функция для преобразования записи из инфиксной записи в обратную польскую 
std::vector<std::string> to_rpn(const std::string& expression) {
	std::vector<std::string> output_queue;
	std::stack<std::string> operator_stack;

	// Разделение строки на пробелы
	std::stringstream  ss(expression);
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