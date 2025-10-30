#include "Calculator.h"
#include <windows.h>
#include <filesystem>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <cmath> 
#include <cctype> 
#include <stdexcept>
#include <charconv>
#include <system_error>

Calculator::Calculator() {}

Calculator::~Calculator() {
    for (void* handle : plugin_handles) {
        if (handle) {
            FreeLibrary(static_cast<HMODULE>(handle));
        }
    }
}

// Загрузка плагинов
void Calculator::loadPlugins() {
    std::string pluginDir = "./plugins";
    int loadedCount = 0; 

    if (!std::filesystem::exists(pluginDir) || !std::filesystem::is_directory(pluginDir)) {
        try {
            std::filesystem::create_directory(pluginDir);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Ошибка при создании директории 'plugins': " << e.what() << std::endl;
            return; 
        }
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(pluginDir)) {
        if (entry.path().extension() == ".dll") {

            HMODULE handle = LoadLibraryW(entry.path().wstring().c_str());
            if (handle == NULL) {
                DWORD error = GetLastError();
                std::cerr << "Ошибка загрузки DLL: " << entry.path().string() << " (Код ошибки: " << error << ")" << std::endl;
                continue;
            }

            auto getFuncName = (const char* (*)())GetProcAddress(handle, "getFunctionName");
            auto executeUnary = (UnaryFunctionPtr)GetProcAddress(handle, "execute");
            auto executeBinary = (BinaryFunctionPtr)GetProcAddress(handle, "executeBinary");
            auto getArity = (int(*)())GetProcAddress(handle, "getFunctionArity");

            if (getFuncName == nullptr) {
                std::cerr << "Ошибка: Не найдена функция getFunctionName в " << entry.path().string() << std::endl;
                FreeLibrary(handle); 
                continue;
            }

            std::string funcName = getFuncName();
            if (functions.count(funcName)) {
                std::cerr << "Предупреждение: Функция '" << funcName << "' из " << entry.path().string() << " уже загружена. Пропуск." << std::endl;
                FreeLibrary(handle); 
                continue;
            }

            FunctionEntry entryInfo{};

            if (getArity) {
                int reportedArity = getArity();
                if (reportedArity == 1) {
                    entryInfo.arity = 1;
                    entryInfo.unary = executeUnary;
                }
                else if (reportedArity == 2) {
                    entryInfo.arity = 2;
                    entryInfo.binary = executeBinary;
                }
                else {
                    std::cerr << "Ошибка: Плагин '" << funcName << "' сообщил неподдерживаемую арность: " << reportedArity << std::endl;
                }
            }

            if (entryInfo.arity == 0) {
                if (executeBinary && !executeUnary) {
                    entryInfo.arity = 2;
                    entryInfo.binary = executeBinary;
                }
                else if (executeUnary) {
                    entryInfo.arity = 1;
                    entryInfo.unary = executeUnary;
                }
            }

            if (entryInfo.arity == 1 && entryInfo.unary == nullptr) {
                std::cerr << "Ошибка: Плагин '" << funcName << "' не предоставляет execute для арности 1." << std::endl;
            }
            else if (entryInfo.arity == 2 && entryInfo.binary == nullptr) {
                std::cerr << "Ошибка: Плагин '" << funcName << "' не предоставляет executeBinary для арности 2." << std::endl;
            }

            if (entryInfo.arity == 0 || (entryInfo.arity == 1 && entryInfo.unary == nullptr) || (entryInfo.arity == 2 && entryInfo.binary == nullptr)) {
                FreeLibrary(handle);
                continue;
            }

            functions[funcName] = entryInfo;
            plugin_handles.push_back(handle);
            loadedCount++;
        }
    }
    if (loadedCount == 0) {
        std::cout << "Плагины не найдены в директории '" << pluginDir << "'." << std::endl;
    }
    else {
        std::cout << "Загружено плагинов: " << loadedCount << std::endl;
    }
}

double Calculator::calculate(const std::string& expression) {
    try {
        std::vector<std::string> rpn = to_rpn(expression);
        return evaluate_rpn(rpn);
    }
    catch (const std::exception& e) {
        throw; 
    }
}


std::vector<std::string> Calculator::to_rpn(const std::string& expression) {
    std::vector<std::string> output_queue;
    std::stack<std::string> operator_stack;
    TokenType last_token_type = TokenType::NONE; 

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isspace(c)) {
            continue; // Игнорируем пробелы
        }

        // Обработка чисел 
        bool is_unary = (c == '-' || c == '+') &&
            (last_token_type == TokenType::NONE ||
                last_token_type == TokenType::LEFT_PAREN ||
                last_token_type == TokenType::OPERATOR);

        if (isdigit(c) || (c == '.' && i + 1 < expression.length() && isdigit(expression[i + 1])) || is_unary) {
            std::string number_str;
            if (is_unary) {
                number_str += c;
                i++; 
            }
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                number_str += expression[i];
                i++;
            }
            i--; 

            double value = 0.0;
            if (!try_parse_number(number_str, value)) {
                throw std::runtime_error("Некорректный формат числа: " + number_str);
            }

            output_queue.push_back(number_str);
            last_token_type = TokenType::NUMBER;
        }
        // Обработка имен функций 
        else if (isalpha(c)) {
            std::string name;
            while (i < expression.length() && isalnum(expression[i])) { 
                name += expression[i];
                i++;
            }
            i--; 

            if (functions.count(name)) { 
                operator_stack.push(name);
                last_token_type = TokenType::FUNCTION;
            }
            else {
                throw std::runtime_error("Неизвестная функция или переменная: " + name); 
            }
        }
        // --- Обработка операторов ---
        else if (is_operator(std::string(1, c))) {
            std::string current_op(1, c);
            // Обработка для унарного плюса 
            if (current_op == "+" && (last_token_type == TokenType::NONE || last_token_type == TokenType::LEFT_PAREN || last_token_type == TokenType::OPERATOR)) {
                continue;
            }

            while (!operator_stack.empty() && operator_stack.top() != "(" &&
                !functions.count(operator_stack.top()) && 
                get_precedence(operator_stack.top()) >= get_precedence(current_op)) {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(current_op);
            last_token_type = TokenType::OPERATOR;
        }
        // Обработка скобок 
        else if (c == '(') {
            operator_stack.push("(");
            last_token_type = TokenType::LEFT_PAREN;
        }
        else if (c == ')') {
            while (!operator_stack.empty() && operator_stack.top() != "(") {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }
            if (operator_stack.empty()) {
                throw std::runtime_error("Ошибка: Несогласованные скобки (лишняя закрывающая).");
            }
            operator_stack.pop(); // Выкидываем '('
            last_token_type = TokenType::RIGHT_PAREN;

            // Если перед скобкой была функция, выталкиваем ее
            if (!operator_stack.empty() && functions.count(operator_stack.top())) {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }
        }
        else {
            throw std::runtime_error(std::string("Неизвестный символ в выражении: ") + c);
        }
    }

    // Выталкиваем все оставшиеся операторы из стека
    while (!operator_stack.empty()) {
        if (operator_stack.top() == "(") {
            throw std::runtime_error("Ошибка: Несогласованные скобки (лишняя открывающая).");
        }
        output_queue.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return output_queue;
}

double Calculator::evaluate_rpn(const std::vector<std::string>& rpn_tokens) {
    std::stack<double> value_stack;

    for (const std::string& token : rpn_tokens) {
        double number = 0.0;
        if (try_parse_number(token, number)) {
            value_stack.push(number);
            continue;
        }

        auto funcIt = functions.find(token);
        if (funcIt != functions.end()) {
            const auto& entry = funcIt->second;

            if (entry.arity == 1) {
                if (value_stack.empty()) {
                    throw std::runtime_error("Ошибка: Недостаточно аргументов для функции '" + token + "'");
                }

                double operand = value_stack.top();
                value_stack.pop();

                try {
                    value_stack.push(entry.unary(operand));
                }
                catch (const std::exception& e) {
                    throw std::runtime_error("Ошибка при вычислении функции '" + token + "': " + e.what());
                }
                catch (...) {
                    throw std::runtime_error("Неизвестная ошибка при вычислении функции '" + token + "'");
                }
            }
            else if (entry.arity == 2) {
                if (value_stack.size() < 2) {
                    throw std::runtime_error("Ошибка: Недостаточно аргументов для функции '" + token + "'");
                }

                double operand2 = value_stack.top(); value_stack.pop();
                double operand1 = value_stack.top(); value_stack.pop();

                try {
                    value_stack.push(entry.binary(operand1, operand2));
                }
                catch (const std::exception& e) {
                    throw std::runtime_error("Ошибка при вычислении функции '" + token + "': " + e.what());
                }
                catch (...) {
                    throw std::runtime_error("Неизвестная ошибка при вычислении функции '" + token + "'");
                }
            }
            else {
                throw std::runtime_error("Ошибка: Неподдерживаемая арность функции '" + token + "'");
            }
        }
        // Если токен - известный оператор
        else if (is_operator(token)) {
            if (value_stack.size() < 2) {
                throw std::runtime_error("Ошибка: Недостаточно операндов для оператора '" + token + "'");
            }
            double operand2 = value_stack.top(); value_stack.pop();
            double operand1 = value_stack.top(); value_stack.pop();

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
                    throw std::runtime_error("Ошибка: Деление на ноль.");
                }
                value_stack.push(operand1 / operand2);
            }
            else {
                throw std::runtime_error("Ошибка: Неизвестный оператор '" + token + "'");
            }
        }
        else {
            throw std::runtime_error("Ошибка: Неизвестный токен в RPN: " + token);
        }
    }

    if (value_stack.size() != 1) {
        throw std::runtime_error("Ошибка: Неверный формат выражения");
    }

    return value_stack.top(); 
}

// Вспомогательная функция для проверки, является ли строка оператором
bool Calculator::is_operator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

// Вспомогательная функция для определения приоритета оператора
int Calculator::get_precedence(const std::string& op) {
    static const std::unordered_map<std::string, int> precedence = {
        {"+", 1}, {"-", 1},
        {"*", 2}, {"/", 2},
        {"^", 3} 
    };

    // Функции имеют самый высокий приоритет при разборе стека операторов
    if (functions.count(op)) {
        return 4; 
    }

    if (op == "(" || op == ")") {
        return 0;
    }

    auto it = precedence.find(op);
    return (it != precedence.end()) ? it->second : -1; 
}

bool Calculator::try_parse_number(const std::string& token, double& value) {
    const char* begin = token.data();
    const char* end = token.data() + token.size();

    auto result = std::from_chars(begin, end, value, std::chars_format::general);
    if (result.ec == std::errc::result_out_of_range) {
        throw std::runtime_error("Число вне допустимого диапазона: " + token);
    }

    return (result.ec == std::errc() && result.ptr == end);
}

