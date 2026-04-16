cmake_minimum_required(VERSION 3.15)

project(ScientificCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CALCULATOR_DIR ${CMAKE_SOURCE_DIR}/lab1_Calculator)

set(OUTPUT_BASE_DIR ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${OUTPUT_BASE_DIR}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${OUTPUT_BASE_DIR}/Release)

if(NOT MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BASE_DIR})
endif()

add_executable(calc 
    "${CALCULATOR_DIR}/src/main.cpp"
    "${CALCULATOR_DIR}/src/Calculator.cpp"
    "${CALCULATOR_DIR}/src/PluginLoader.cpp"
    "${CALCULATOR_DIR}/src/ExpressionParser.cpp"
)

target_include_directories(calc PRIVATE 
    "${CALCULATOR_DIR}/include"
)

if(WIN32)
  target_link_libraries(calc PRIVATE kernel32)
  target_compile_definitions(calc PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

add_custom_command(TARGET calc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CALCULATOR_DIR}/plugins" 
        "$<TARGET_FILE_DIR:calc>/plugins" 
    COMMENT "Copying plugins directory to build output")