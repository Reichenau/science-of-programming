# Лабораторная работа 2: Обертка (Wrapper)
Выполнил: Шильдт Даниэль 5030102/30202

## Задание

1. Реализовать инкапсуляцию методов класса произвольной сигнатуры.
2. Реализовать код юнит-теста:
```
Wrapper wrapper(&subj, &Subject::f3, {{"arg1", 0}, {"arg2", 0}});

Engine engine;

engine.register_command(&wrapper, "command1");

std::cout << engine.execute("command1", { {"arg1", 4}, {"arg2", 5} }) << std::endl;
// Этот фрагмент из задания, но для запуска нужен any_cast (см. ниже)
std::any_cast<int>(engine.execute("command1", { {"arg1", 4}, {"arg2", 5} }));
```
## Описание

Программа позволяет:
1. Обернуть метод произвольного класса (`Subject`) в объект-команду (`Wrapper`).
2. Задать аргументы по умолчанию при создании команды.
3. Зарегистрировать команду в движке (`Engine`).
4. Вызвать команду по имени, передав новые аргументы.

### Структура проекта 

lab_2_wrapper/ 
├── include/ 
│ 
├── engine/ 
│ 
│ └── Engine.hpp			   // Объявление класса Engine 
│ ├── subject/ 
│ │ └── Subject.hpp			   // Тестовый класс с методами 
│ └── wrapper/ 
│ ├── Wrapper.hpp              // Абстрактный интерфейс обертки
│ └── WrapperImpl.hpp          // Шаблонная реализация обертки
├── src/ 
│ ├── engine/ 
│ │ └── Engine.cpp 
│ ├── main.cpp                 // Точка входа: запускает все модульные тесты 
│ └── subject/ 
│ └── Subject.cpp 
├── tests/ 
│ ├── test.hpp                 // Объявление функции run_all_tests() 
│ └── testWrapper.cpp          // Реализация 6 модульных тестов 
├── .gitignore 
├── CMakeLists.txt 
└── README.md

### Архитектура

* **Subject** — класс (метод `f3`).
* **Wrapper (интерфейс)** — абстрактный базовый класс для унификации хранения команд.
* **WrapperImpl (шаблон)** — конкретная реализация обертки. Хранит указатель на объект, метод и аргументы.
* **Engine** — менеджер команд. Хранит реестр команд (`std::map`) и управляет их вызовом.

## Тестирование (Unit Tests)

| ID | Описание теста | Проверяемая функциональность |
| :--- | :--- | :--- |
| **Test 1** | Basic Execution (int) | Корректный вызов метода с возвращаемым значением (`Subject::f3`). |
| **Test 2** | Void Method Execution | Корректный вызов метода `void` (`Subject::print`) и возврат пустого `std::any`. |
| **Test 3** | Default Values Override | Использование значений по умолчанию, когда часть аргументов передана, а часть берется из `defaults`. |
| **Test 4** | Error: Command Not Found | Обработка исключения `std::runtime_error`, когда команда не зарегистрирована в `Engine`. |
| **Test 5** | Error: Missing Required Arg | Обработка исключения `std::invalid_argument`, когда аргумент не передан и отсутствует в `defaults`. |
| **Test 6** | Error: Return Type Mismatch | Обработка исключения `std::bad_any_cast` при попытке извлечь результат неверного типа. |

## Требования

* C++17 (используется `std::index_sequence`, `std::map`, `if constexpr` features).
* CMake 3.10+.

## Сборка и запуск

Сборка осуществляется через CMake.

### Консоль 

```bash
git clone https://github.com/Reichenau/science-of-programming.git
cd science-of-programming
git checkout lab_2_wrapper

mkdir build
cd build
cmake ..
cmake --build .

# Запуск на Windows (Visual Studio)
./Debug/lab_2_wrapper.exe

# Запуск на Linux / macOS
./lab_2_wrapper
```
