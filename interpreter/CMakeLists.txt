cmake_minimum_required(VERSION 3.15)
project(interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(interpreter main.cpp Code_Interpreter.h Code_Interpreter.cpp)