cmake_minimum_required(VERSION 3.31)

project(brainfuck-compiler
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  include(gcc_modules.cmake)
endif()

add_executable(brainfuck-compiler)

target_sources(brainfuck-compiler
  PUBLIC
    src/main.cpp
)

target_sources(brainfuck-compiler
  PUBLIC
    FILE_SET modules
    TYPE CXX_MODULES
    FILES
      src/lexer.cppm
      src/ir.cppm
      src/utils.cppm
)
