cmake_minimum_required(VERSION 3.27)
project(Kodama)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kodama src/main.cpp
        src/lexer/lexer.hpp
        src/lexer/lexer.hpp
        src/lexer/lexer.cpp
        src/lexer/token.hpp
        src/ast/ast.hpp
        src/parser/parser.cpp
        src/parser/parser.hpp
        src/ast/ast.hpp
        src/ast/ast.cpp)