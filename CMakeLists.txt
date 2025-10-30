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

message(STATUS "Build output base directory: ${OUTPUT_BASE_DIR}")


message(STATUS "Configuring Calculator executable...")
add_executable(calc 
    "${CALCULATOR_DIR}/src/main.cpp"
    "${CALCULATOR_DIR}/src/Calculator.cpp"
)

target_include_directories(calc PRIVATE 
    "${CALCULATOR_DIR}/include"
)

if(WIN32)
  target_link_libraries(calc PRIVATE kernel32)
  target_compile_definitions(calc PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()

set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)

message(STATUS "Configuring PowerPlugin dynamic library...")
add_library(PowerPlugin SHARED "${CALCULATOR_DIR}/plugins_src/PowerPlugin.cpp")
set_target_properties(PowerPlugin PROPERTIES
    OUTPUT_NAME "PowerPlugin"
    RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
)

add_custom_command(TARGET PowerPlugin POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CALCULATOR_DIR}/plugins"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:PowerPlugin>
        "${CALCULATOR_DIR}/plugins/$<TARGET_FILE_NAME:PowerPlugin>"
    COMMENT "Copying PowerPlugin into source plugins directory")

add_dependencies(calc PowerPlugin)

add_custom_command(TARGET calc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
        "${CALCULATOR_DIR}/plugins" 
        "$<TARGET_FILE_DIR:calc>/plugins" 
    COMMENT "Copying existing plugins directory to build output")


message(STATUS "CMake configuration complete.")