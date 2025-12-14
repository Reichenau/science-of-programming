#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <any>
#include <stdexcept>
#include <sstream>
#include <functional>

#include "engine/Engine.hpp"
#include "subject/Subject.hpp" 
#include "wrapper/WrapperImpl.hpp"
#include "test.hpp"

void log_test(const std::string& name, std::function<void()> test_func) {
    std::cout << "--- TEST: " << name << " ---\n";
    try {
        test_func();
        std::cout << ">>> RESULT: [SUCCESS]\n";
    }
    catch (const std::exception& e) {
        std::cout << ">>> RESULT: [FAILURE] Exception: " << e.what() << "\n";
    }
    catch (...) {
        std::cout << ">>> RESULT: [FAILURE] Unknown error\n";
    }
}

template<typename R, typename... Args>
std::unique_ptr<IWrapper> makeWrapper(Subject* subj, R(Subject::* method)(Args...), const std::map<std::string, std::any>& defaults = {}) {
    return std::make_unique<Wrapper<Subject, R, Args...>>(subj, method, defaults);
}

// Тест 1: Базовая целочисленная операция (a - b)
void test1_basic_execution() {
    Subject subj;
    Engine engine;
    engine.register_command(
        makeWrapper(&subj, &Subject::f3, { {"arg1", 0}, {"arg2", 0} }),
        "calc_diff"
    );

    // f3(4, 5) -> 4 - 5 = -1
    auto result = engine.execute("calc_diff", { {"arg1", 4}, {"arg2", 5} });
    int actual = std::any_cast<int>(result);
    if (actual != -1) {
        throw std::runtime_error("Expected -1, got " + std::to_string(actual));
    }
}

// Тест 2: Обработка void (Subject::print)
void test2_void_method() {
    Subject subj;
    Engine engine;
    engine.register_command(
        makeWrapper(&subj, &Subject::print, { {"message", std::string("default")} }),
        "print_log"
    );
    engine.execute("print_log", { {"message", std::string("Test message")} });
}

// Тест 3: Использование значения по умолчанию
void test3_default_values() {
    Subject subj;
    Engine engine;
    engine.register_command(
        makeWrapper(&subj, &Subject::f3, { {"arg1", 10}, {"arg2", 3} }),
        "diff_defaults"
    );

    auto result = engine.execute("diff_defaults", { {"arg1", 20} });
    int actual = std::any_cast<int>(result);
    if (actual != 17) {
        throw std::runtime_error("Expected 17, got " + std::to_string(actual));
    }
}

// Тест 4: Ошибка: Команда не найдена
void test4_command_not_found() {
    Engine engine;
    bool exception_caught = false;
    try {
        engine.execute("non_existent_command", {});
    }
    catch (const std::runtime_error&) {
        exception_caught = true;
    }
    if (!exception_caught) {
        throw std::runtime_error("Expected runtime_error for missing command.");
    }
}

// Тест 5: Ошибка: Отсутствует обязательный аргумент
void test5_missing_required_argument() {
    Subject subj;
    Engine engine;
    engine.register_command(
        makeWrapper(&subj, &Subject::f3),
        "diff_no_defaults"
    );

    bool exception_caught = false;
    try {
        engine.execute("diff_no_defaults", { {"arg1", 1} });
    }
    catch (const std::invalid_argument&) {
        exception_caught = true;
    }
    if (!exception_caught) {
        throw std::runtime_error("Expected invalid_argument for missing arg.");
    }
}

// Тест 6: Ошибка: Несовпадение типа возвращаемого значения
void test6_return_type_mismatch() {
    Subject subj;
    Engine engine;
    engine.register_command(
        makeWrapper(&subj, &Subject::f3),
        "calc_diff"
    );

    bool exception_caught = false;
    try {
        std::any_cast<std::string>(engine.execute("calc_diff", { {"arg1", 1}, {"arg2", 1} }));
    }
    catch (const std::bad_any_cast&) {
        exception_caught = true;
    }
    if (!exception_caught) {
        throw std::runtime_error("Expected bad_any_cast.");
    }
}


void run_all_tests() {
    log_test("Basic Execution (int)", test1_basic_execution);
    log_test("Void Method Execution", test2_void_method);
    log_test("Default Values Override", test3_default_values);
    log_test("Error: Command Not Found", test4_command_not_found);
    log_test("Error: Missing Required Arg", test5_missing_required_argument);
    log_test("Error: Return Type Mismatch", test6_return_type_mismatch);
}