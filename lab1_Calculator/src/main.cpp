#include <iostream>
#include <locale.h>
#include "parser.h"
#include "evaluator.h"

void print_tokens(const std::vector<std::string>& tokens) {
    for (const auto& token : tokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
}
int main(){
    setlocale(LC_ALL, "ru");
    
    std::string expression = "16 + 4 * ( 3 - 1 )";
    std::vector<std::string> rpn_expression = to_rpn(expression);
    double result = evaluate_rpn(rpn_expression);

    std::cout << expression << std::endl;

    std::cout << "Результат: " << result;

    return 0;
}


