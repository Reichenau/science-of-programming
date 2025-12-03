#include <iostream>
#include <any>
#include "subject/Subject.hpp"      
#include "engine/Engine.hpp"        
#include "wrapper/WrapperImpl.hpp"

int main() {
    // Создаем объекты
    Subject subj;
    Engine engine;

    Wrapper wrapper1(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    Wrapper wrapper2(&subj, &Subject::print, { {"arg1", std::string("default")} });

    // Создаем обертки для методов
    try {
        // Регистрируем команды
        engine.register_command(&wrapper1, "command1");
        engine.register_command(&wrapper2, "command2");

        // Тест 1 (int)
        auto result = engine.execute("command1", { {"arg1", 4}, {"arg2", 5} });
        std::cout << "Result: " << std::any_cast<int>(result) << std::endl;

        // Тест 2 (string, void)
        engine.execute("command2", { {"arg1", std::string("Hello World!")} });
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}