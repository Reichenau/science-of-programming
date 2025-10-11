#include <iostream>
#include <parser.h>
#include <locale.h>
void print_tokens(const std::vector<std::string>& tokens) {
    for (const auto& token : tokens) {
        std::cout << token << " ";
    }
    std::cout << std::endl;
}
int main()
{
    setlocale(LC_ALL, "ru");
    std::string expression = "16 + 4 * ( 3 - 1 )";

    std::cout << expression << std::endl;

    std::vector<std::string> rpn_expression = to_rpn(expression);

    std::cout << "Результат: ";
    print_tokens(rpn_expression);

    return 0;
}


