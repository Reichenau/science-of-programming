cmake_minimum_required(VERSION 3.10)

project(lab_2_wrapper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab_2_wrapper src/main.cpp src/Subject.cpp)