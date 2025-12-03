cmake_minimum_required(VERSION 3.10)

project(lab_2_wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(core_lib 
    src/engine/Engine.cpp 
    src/subject/Subject.cpp
)

if(MSVC)
    target_compile_options(core_lib PRIVATE /W4)
else()
    target_compile_options(core_lib PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(lab_2_wrapper src/main.cpp)

target_link_libraries(lab_2_wrapper PRIVATE core_lib)