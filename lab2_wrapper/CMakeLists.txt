cmake_minimum_required(VERSION 3.10)

project(lab_2_wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
include_directories(tests) 

add_library(core_lib 
    src/engine/Engine.cpp 
    src/subject/Subject.cpp
    tests/testWrapper.cpp 
)

add_executable(lab_2_wrapper src/main.cpp)
target_link_libraries(lab_2_wrapper PRIVATE core_lib)

