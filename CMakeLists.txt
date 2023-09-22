cmake_minimum_required(VERSION 3.27)
project(Kodama)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g")

find_package(LLVM 16 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Code
add_executable(Kodama
        src/main.cpp
        src/lexer/lexer.hpp
        src/lexer/lexer.hpp
        src/lexer/lexer.cpp
        src/lexer/token.hpp
        src/ast/ast.hpp
        src/parser/parser.cpp
        src/parser/parser.hpp
        src/ast/ast.hpp
        src/ast/ast.cpp
        src/ast/astVisitor.cpp
        src/ast/astVisitor.hpp
        src/codegen/codegen.cpp
        src/codegen/codegen.hpp
        src/codegen/irtransform.cpp
        src/codegen/irtransform.hpp
        src/errors/error.cpp
        src/errors/error.hpp
        src/utils/stringUtils.cpp
        src/utils/stringUtils.hpp
        src/analyzer/type/typeChecker.cpp
        src/analyzer/type/typeChecker.hpp
        src/analyzer/type/datatype.cpp
        src/analyzer/type/datatype.hpp)

# Find the libraries that correspond to the LLVM components
# that you wish to use
llvm_map_components_to_libnames(llvm_libs support core)

# Link against LLVM libraries
target_link_libraries(Kodama LLVM-16)

# Add a custom target to run the lli command after building
add_custom_target(run_llvm
        COMMAND lli "${PROJECT_SOURCE_DIR}/out/out.ll"
        DEPENDS Kodama
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running LLVM IR"
)

# Make sure the custom target is executed after building
add_dependencies(run_llvm Kodama)

# Tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(tests test/test.cpp)
target_link_libraries(tests gtest gtest_main)
add_test(NAME SquareTest COMMAND SquareTest)