#include <iostream>
#include <any>
#include <memory>
#include "subject/Subject.hpp"      
#include "engine/Engine.hpp"        
#include "wrapper/WrapperImpl.hpp"

int main() {
    std::cout << "=== Lab work: Wrapper ===" << std::endl;

    Subject subj;
    Engine engine;

    auto wrapper1 = std::make_unique<WrapperImpl<Subject, int, int, int>>(
        &subj, &Subject::f3, std::map<std::string, std::any>{ {"arg1", 0}, { "arg2", 0 } }
    );
    auto wrapper2 = std::make_unique<WrapperImpl<Subject, void, const std::string&>>(
        &subj, &Subject::print, std::map<std::string, std::any>{ {"arg1", std::string("default")} }
    );

    try {
        engine.register_command(std::move(wrapper1), "command1");
        engine.register_command(std::move(wrapper2), "command2");

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