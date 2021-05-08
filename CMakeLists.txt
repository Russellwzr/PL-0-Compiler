cmake_minimum_required(VERSION 3.15)
project(Compiler_Lab)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler_Lab main.cpp Lexical_Analyzer.h utils.h Syntax_Analyzer.h Syntax_Analyzer.cpp)