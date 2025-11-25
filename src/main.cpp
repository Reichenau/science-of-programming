#include <iostream>
#include "subject/Subject.hpp"      
#include "engine/Engine.hpp"        
#include "wrapper/WrapperImpl.hpp"

int main() {
    std::cout << "=== Lab work: Wrapper ===" << std::endl;

    Subject subj;
    Engine engine;

    WrapperImpl<Subject, int, int, int> wrapper1(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} });
    WrapperImpl<Subject, void, const std::string&> wrapper2(&subj, &Subject::print, { {"arg1", std::string("default")} });

    try {
        engine.register_command(&wrapper1, "command1");
        engine.register_command(&wrapper2, "command2");

        // Тест 1 (int)
        std::any res1 = engine.execute("command1", { {"arg1", 10}, {"arg2", 3} });
        std::cout << "Result: " << std::any_cast<int>(res1) << std::endl;

        // Тест 2 (string, void)
        engine.execute("command2", { {"arg1", std::string("Hello, World!")} });
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}