#include <iostream>
#include "Subject.hpp"
#include "Engine.hpp"
#include "WrapperImpl.hpp"

int main() {
    std::cout << "=== Lab work: Wrapper ===" << std::endl;

    Subject subj;
    Engine engine;

    WrapperImpl<Subject, int, int> wrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });

    try {
        engine.register_command(&wrapper, "command1");

        // Тест 1
        std::cout << "Result (4, 5): "
            << engine.execute("command1", { {"arg1", 4}, {"arg2", 5} })
            << std::endl;

        // Тест 2
        std::cout << "Result (10, default): "
            << engine.execute("command1", { {"arg1", 10} })
            << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}