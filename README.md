## Структура репозитория

```text
science-of-programming/
├── CMakeLists.txt
├── README.md
├── lab1_Calculator/
│   ├── CMakeLists.txt
│   ├── include/
│   ├── plugins/
│   └── src/
└── lab2_wrapper/
	├── CMakeLists.txt
	├── include/
	├── src/
	└── tests/
```

## Лабораторная работа 1: Dynamic Calculator

Каталог: `lab1_Calculator/`

Задача: консольный калькулятор с операциями `+ - / * ()` и динамической загрузкой
функций из DLL-плагинов из папки `plugins`.

Ключевые требования:
1. Расширение функционала через DLL на этапе выполнения.
2. Каждый плагин содержит одну функцию.
3. Работа программы даже при отсутствии или ошибке отдельных плагинов.

## Лабораторная работа 2: Wrapper

Каталог: `lab2_wrapper/`

Задача: обертка методов произвольной сигнатуры, регистрация команд в движке и
вызов по имени через общий интерфейс.

Ключевые компоненты:
1. `Subject` - демонстрационный класс с методами.
2. `IWrapper`/`Wrapper` - абстракция и шаблонная реализация обертки.
3. `Engine` - реестр команд и маршрутизация вызова.
4. Набор unit-тестов в `tests/`.

## Сборка

Сборка обеих лабораторных:

```bash
cmake -S . -B build
cmake --build build
```

Сборка только Lab1:

```bash
cmake -S . -B build -DBUILD_LAB1=ON -DBUILD_LAB2=OFF
cmake --build build
```

Сборка только Lab2:

```bash
cmake -S . -B build -DBUILD_LAB1=OFF -DBUILD_LAB2=ON
cmake --build build
```

