cmake_minimum_required(VERSION 3.10)
project(python_from_cpp)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

add_executable(python_from_cpp main.cpp)
target_link_libraries(python_from_cpp PRIVATE Python3::Python)
