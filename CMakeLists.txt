cmake_minimum_required(VERSION 3.15)

project(science_of_programming LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_LAB1 "Build lab1 calculator" ON)
option(BUILD_LAB2 "Build lab2 wrapper" ON)

if(BUILD_LAB1)
    add_subdirectory(lab1_Calculator)
endif()

if(BUILD_LAB2)
    add_subdirectory(lab2_wrapper)
endif()