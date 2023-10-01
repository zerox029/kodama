cmake_minimum_required(VERSION 3.27)
project(Kodama)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak -g")

find_package(LLVM 16 REQUIRED CONFIG)
find_package(fmt CONFIG REQUIRED)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

# Set source and output directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/kodama)
set(OUT_DIR ${CMAKE_SOURCE_DIR}/out)

# Collect source files
file(GLOB_RECURSE SRC_FILES
        ${SRC_DIR}/analyzer/*.cpp
        ${SRC_DIR}/analyzer/*.hpp
        ${SRC_DIR}/ast/*.cpp
        ${SRC_DIR}/ast/*.hpp
        ${SRC_DIR}/codegen/*.cpp
        ${SRC_DIR}/codegen/*.hpp
        ${SRC_DIR}/compiler/*.cpp
        ${SRC_DIR}/compiler/*.hpp
        ${SRC_DIR}/errors/*.cpp
        ${SRC_DIR}/errors/*.hpp
        ${SRC_DIR}/lexer/*.cpp
        ${SRC_DIR}/lexer/*.hpp
        ${SRC_DIR}/parser/*.cpp
        ${SRC_DIR}/parser/*.hpp
        ${SRC_DIR}/utils/*.cpp
        ${SRC_DIR}/utils/*.hpp)

add_executable(Kodama
        ${SRC_FILES}
        ${SRC_DIR}/main.cpp)

# Find the libraries that correspond to the LLVM components
# that you wish to use
llvm_map_components_to_libnames(llvm_libs support core)

# Link against LLVM libraries
target_link_libraries(Kodama LLVM-16 fmt::fmt)

# Add a custom target to run the lli command after building
add_custom_target(run_llvm
        COMMAND lli "${PROJECT_SOURCE_DIR}/out/out.ll"
        DEPENDS Kodama
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running LLVM IR"
)

# Make sure the custom target is executed after building
add_dependencies(run_llvm Kodama)

## Tests
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Collect source files
file(GLOB_RECURSE TEST_FILES
        ${SRC_DIR}/test/*.cpp
        ${SRC_DIR}/test/*.hpp)

add_executable(tests ${SRC_FILES} ${TEST_FILES}
        kodama/test/parser/primaryTests.cpp)

target_link_libraries(tests gtest gtest_main LLVM-16 fmt::fmt)