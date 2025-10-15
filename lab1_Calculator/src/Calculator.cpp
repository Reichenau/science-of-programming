#include "Calculator.h"
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <stack>
#include <sstream>
#include <unordered_map>
#include <stdexcept>

Calculator::Calculator(){
}

Calculator::~Calculator() {
	for (void* handle : plugin_handles) {
		FreeLibrary((HMODULE)handle);
	}
}

// ЗАгрузка плагинов
void Calculator::loadPlugins() {
	std::string pluginDir = "./plugins"; // Путь к папке с плагинами .dll

	// Проверка на то есть ли такая папка
	if (!std::filesystem::exists(pluginDir) || !std::filesystem::is_directory(pluginDir)) {
		std::cout << "Ошибка: директория не найдела." << std::endl;
		std::filesystem::create_directory(pluginDir);
		return;
	}

	// Поиск всех файлов с расширением .dll
	for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
		if (entry.path().extension() == ".dll") {
			std::cout << "Загружка плагина: " << entry.path().string() << std::endl;

			// Загрудка DLL в память 
			HMODULE handle = LoadLibrary(entry.path().c_str());
			if (handle == NULL) {
				std::cerr << "Ошибка загрузки DLL: " << entry.path().string() << std::endl;
				continue;
			}

			// Поиск в DLL функции с именем getFunctionName
			auto getFuncName = (const char* (*)())GetProcAddress(handle, "getFunctionName");
				
			// Поиск в DLL функции с именем execute
			auto func = (FunctionPtr)GetProcAddress(handle, "execute");
			
			if (getFuncName == nullptr || func == nullptr) {
				std::cerr << "Не удалось найти функции в " << entry.path().string() << std::endl;
				FreeLibrary(handle);
				continue;
			}

			// Получаем имя функции и сохраняем ее
			std::string funcName = getFuncName();
			functions[funcName] = func;

			// Сохраняем хэнд, чтобы потом освободить память 
			plugin_handles.push_back(handle);
			std::cout << "Функция загружена " << funcName << std::endl;
		}
	}
}

double Calculator::calculate(const std::string& expression) {
	std::vector<std::string> rpn = to_rpn(expression);
	return evaluate_rpn(rpn);
}

// Реализация парсера 
// src/Calculator.cpp

std::vector<std::string> Calculator::to_rpn(const std::string& expression) {
	std::vector<std::string> output_queue;
	std::stack<std::string> operator_stack;

	for (size_t i = 0; i < expression.length(); ++i) {
		char c = expression[i];

		if (isspace(c)) {
			continue; // Пропускаем пробелы
		}
		// Если символ - цифра, считываем всё число целиком
		else if (isdigit(c)) {
			std::string number;
			while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
				number += expression[i];
				i++;
			}
			i--; // Возвращаемся на один символ назад, т.к. внешний цикл сделает i++
			output_queue.push_back(number);
		}
		// Если символ - буква, считываем всё имя функции
		else if (isalpha(c)) {
			std::string func_name;
			while (i < expression.length() && isalpha(expression[i])) {
				func_name += expression[i];
				i++;
			}
			i--;
			if (functions.count(func_name)) {
				operator_stack.push(func_name);
			}
			else {
				// В будущем здесь можно бросить ошибку о неизвестной функции
			}
		}
		else if (c == '(') {
			operator_stack.push("(");
		}
		else if (c == ')') {
			while (!operator_stack.empty() && operator_stack.top() != "(") {
				output_queue.push_back(operator_stack.top());
				operator_stack.pop();
			}
			if (!operator_stack.empty()) operator_stack.pop(); // Выкидываем '('

			// Если после скобки на вершине стека оказалась функция, выталкиваем и её
			if (!operator_stack.empty() && functions.count(operator_stack.top())) {
				output_queue.push_back(operator_stack.top());
				operator_stack.pop();
			}
		}
		else { // Иначе это оператор
			std::string op(1, c);
			while (!operator_stack.empty() && get_precedence(operator_stack.top()) >= get_precedence(op)) {
				output_queue.push_back(operator_stack.top());
				operator_stack.pop();
			}
			operator_stack.push(op);
		}
	}

	// Выталкиваем все оставшиеся операторы из стека
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

		// Если токен - это имя известной функции 
		else if (functions.count(token)) {
			if (value_stack.empty()) {
				throw std::runtime_error("Ошибка: недостаточно операндов для функции");
			}
			double operand = value_stack.top();
			value_stack.pop();

			// Вызов функции из unordered_map
			value_stack.push(functions[token](operand));
		}
		// Токен - оператор
		else {
			if (value_stack.size() < 2) {
				throw std::runtime_error("Ошибка: нет достаточного числа операндов");
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
				value_stack.push(operand1 - operand2);
			}
			else if (token == "*") {
				value_stack.push(operand1 * operand2);
			}
			else if (token == "/") {
				if (operand2 == 0) {
					throw std::runtime_error("Ошибка: деление на 0");
				}
				value_stack.push(operand1 / operand2);
			}
			else if (token == "^") {
				value_stack.push(std::pow(operand1, operand2));
			}
		}
	}

	if (value_stack.size() != 1) {
		throw std::runtime_error("Ошибка: слишком много операндов");
	}

	return value_stack.top(); // Результат вычислений 
}

// Функция для определения приоритета оператора 
int Calculator::get_precedence(const std::string& op) {
	static const std::unordered_map<std::string, int> precedence = {
		{"+", 1}, {"-", 1},
		{"*", 2}, {"/", 2},
		{"^", 3}
	};

	auto it = precedence.find(op);
	return (it != precedence.end()) ? it->second : 0;
}