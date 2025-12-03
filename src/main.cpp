#include <iostream>
#include <any>
#include "subject/Subject.hpp"      
#include "engine/Engine.hpp"        
#include "wrapper/WrapperImpl.hpp"

int main() {
    Subject subj;
    Engine engine;

    Wrapper wrapper1(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    Wrapper wrapper2(&subj, &Subject::print, { {"arg1", std::string("default")} });

    try {
        engine.register_command(&wrapper1, "command1");
        engine.register_command(&wrapper2, "command2");

        // Тест 1 (int)
        std::cout << "Result: " << std::any_cast<int>(engine.execute("command1", { {"arg1", 4}, {"arg2", 5} })) << std::endl;

        // Тест 2 (string, void)
        engine.execute("command2", { {"arg1", std::string("Hello World!")} });
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}