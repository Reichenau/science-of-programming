#include "ExpressionParser.h"
#include <stack>
#include <cmath> 
#include <cctype> 
#include <stdexcept>
#include <charconv>
#include <system_error>
#include <iostream> 

// Главный метод
double ExpressionParser::calculate(const std::string& expression, const std::unordered_map<std::string, FunctionEntry>& functions) {
    try {
        std::vector<std::string> rpn = to_rpn(expression, functions);
        return evaluate_rpn(rpn, functions);
    }
    catch (const std::exception& e) {
        throw; 
    }
}

// Реализация to_rpn 
std::vector<std::string> ExpressionParser::to_rpn(const std::string& expression, const std::unordered_map<std::string, FunctionEntry>& functions) {
    std::vector<std::string> output_queue;
    std::stack<std::string> operator_stack;
    TokenType last_token_type = TokenType::NONE;

    for (size_t i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isspace(c)) {
            continue;
        }

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
        else if (is_operator(std::string(1, c))) {
            std::string current_op(1, c);
            if (current_op == "+" && (last_token_type == TokenType::NONE || last_token_type == TokenType::LEFT_PAREN || last_token_type == TokenType::OPERATOR)) {
                continue;
            }

            while (!operator_stack.empty() && operator_stack.top() != "(" &&
                !functions.count(operator_stack.top()) &&
                get_precedence(operator_stack.top(), functions) >= get_precedence(current_op, functions)) {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }
            operator_stack.push(current_op);
            last_token_type = TokenType::OPERATOR;
        }
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
            operator_stack.pop(); 
            last_token_type = TokenType::RIGHT_PAREN;

            if (!operator_stack.empty() && functions.count(operator_stack.top())) {
                output_queue.push_back(operator_stack.top());
                operator_stack.pop();
            }
        }
        else {
            throw std::runtime_error(std::string("Неизвестный символ в выражении: ") + c);
        }
    }

    while (!operator_stack.empty()) {
        if (operator_stack.top() == "(") {
            throw std::runtime_error("Ошибка: Несогласованные скобки (лишняя открывающая).");
        }
        output_queue.push_back(operator_stack.top());
        operator_stack.pop();
    }

    return output_queue;
}

double ExpressionParser::evaluate_rpn(const std::vector<std::string>& rpn_tokens, const std::unordered_map<std::string, FunctionEntry>& functions) {
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
                double operand = value_stack.top(); value_stack.pop();
                value_stack.push(entry.unary(operand));
            }
            else if (entry.arity == 2) {
                if (value_stack.size() < 2) {
                    throw std::runtime_error("Ошибка: Недостаточно аргументов для функции '" + token + "'");
                }
                double operand2 = value_stack.top(); value_stack.pop();
                double operand1 = value_stack.top(); value_stack.pop();
                value_stack.push(entry.binary(operand1, operand2));
            }
            else {
                throw std::runtime_error("Ошибка: Неподдерживаемая арность функции '" + token + "'");
            }
        }
        else if (is_operator(token)) {
            if (value_stack.size() < 2) {
                throw std::runtime_error("Ошибка: Недостаточно операндов для оператора '" + token + "'");
            }
            double operand2 = value_stack.top(); value_stack.pop();
            double operand1 = value_stack.top(); value_stack.pop();

            if (token == "+") value_stack.push(operand1 + operand2);
            else if (token == "-") value_stack.push(operand1 - operand2);
            else if (token == "*") value_stack.push(operand1 * operand2);
            else if (token == "/") {
                if (operand2 == 0) throw std::runtime_error("Ошибка: Деление на ноль.");
                value_stack.push(operand1 / operand2);
            }
            else throw std::runtime_error("Ошибка: Неизвестный оператор '" + token + "'");
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

bool ExpressionParser::is_operator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

int ExpressionParser::get_precedence(const std::string& op, const std::unordered_map<std::string, FunctionEntry>& functions) {
    static const std::unordered_map<std::string, int> precedence = {
        {"+", 1}, {"-", 1},
        {"*", 2}, {"/", 2},
    };

    if (functions.count(op)) {
        return 4;
    }

    if (op == "(" || op == ")") {
        return 0;
    }

    auto it = precedence.find(op);
    return (it != precedence.end()) ? it->second : -1;
}

bool ExpressionParser::try_parse_number(const std::string& token, double& value) {
    const char* begin = token.data();
    const char* end = token.data() + token.size();

    auto result = std::from_chars(begin, end, value, std::chars_format::general);
    if (result.ec == std::errc::result_out_of_range) {
        throw std::runtime_error("Число вне допустимого диапазона: " + token);
    }

    return (result.ec == std::errc() && result.ptr == end);
}