#include "PluginLoader.h"
#include <windows.h>      
#include <filesystem>
#include <iostream>

PluginLoader::PluginLoader() {}

PluginLoader::~PluginLoader() {
    for (void* handle : plugin_handles) {
        if (handle) {
            FreeLibrary(static_cast<HMODULE>(handle));
        }
    }
}

std::unordered_map<std::string, FunctionEntry> PluginLoader::loadPlugins(const std::string& directoryPath) {
    std::unordered_map<std::string, FunctionEntry> functions;
    int loadedCount = 0;

    if (!std::filesystem::exists(directoryPath) || !std::filesystem::is_directory(directoryPath)) {
        try {
            std::filesystem::create_directory(directoryPath);
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Ошибка при создании директории '" << directoryPath << "': " << e.what() << std::endl;
            return functions; 
        }
        return functions; 
    }

    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".dll") {

            HMODULE handle = LoadLibraryW(entry.path().wstring().c_str());
            if (handle == NULL) {
                DWORD error = GetLastError();
                std::cerr << "Ошибка загрузки DLL: " << entry.path().string() << " (Код ошибки: " << error << ")" << std::endl;
                continue;
            }


            auto getFuncName = (const char* (*)())GetProcAddress(handle, "getFunctionName");
            auto executeUnary = (UnaryFunctionPtr)GetProcAddress(handle, "execute");
            auto executeBinary = (BinaryFunctionPtr)GetProcAddress(handle, "executeBinary");
            auto getArity = (int(*)())GetProcAddress(handle, "getFunctionArity");

            if (getFuncName == nullptr) {
                std::cerr << "Ошибка: Не найдена функция getFunctionName в " << entry.path().string() << std::endl;
                FreeLibrary(handle);
                continue;
            }

            std::string funcName = getFuncName();
            if (functions.count(funcName)) {
                FreeLibrary(handle);
                continue;
            }

            FunctionEntry entryInfo{};

            if (getArity) {
                int reportedArity = getArity();
                if (reportedArity == 1) {
                    entryInfo.arity = 1;
                    entryInfo.unary = executeUnary;
                }
                else if (reportedArity == 2) {
                    entryInfo.arity = 2;
                    entryInfo.binary = executeBinary;
                }
                else {
                    std::cerr << "Ошибка: Плагин '" << funcName << "' сообщил неподдерживаемую арность: " << reportedArity << std::endl;
                }
            }
            if (entryInfo.arity == 0) {
                if (executeBinary && !executeUnary) {
                    entryInfo.arity = 2;
                    entryInfo.binary = executeBinary;
                }
                else if (executeUnary) {
                    entryInfo.arity = 1;
                    entryInfo.unary = executeUnary;
                }
            }


            if (entryInfo.arity == 0 || (entryInfo.arity == 1 && entryInfo.unary == nullptr) || (entryInfo.arity == 2 && entryInfo.binary == nullptr)) {
                std::cerr << "Ошибка: Не удалось определить функцию в " << entry.path().string() << std::endl;
                FreeLibrary(handle);
                continue;
            }

            functions[funcName] = entryInfo;
            plugin_handles.push_back(handle);
            loadedCount++;
        }
    }
    if (loadedCount == 0) {
        std::cout << "Плагины не найдены в директории '" << directoryPath << "'." << std::endl;
    }
    else {
        std::cout << "Загружено плагинов: " << loadedCount << std::endl;
    }

    return functions;
}