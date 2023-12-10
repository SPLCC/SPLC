cmake_minimum_required(VERSION 3.5)
project(SPLC)

find_package(FLEX REQUIRED)

if(NOT FLEX_FOUND)
  message(FATAL_ERROR "FATAL: Flex not found.")
endif()

find_package(BISON 3.6)

if(NOT BISON_FOUND)
  message(FATAL_ERROR "FATAL: Bison not found. At least version 3.6 is required.")
endif()

set(GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(GENERATED_INCL_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR})
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR}/ast)

include_directories(include ${GENERATED_INCL_DIR})
aux_source_directory(src SRC_FILES)

FLEX_TARGET(SPLC_Lexer src/lex.l ${GENERATED_SRC_DIR}/lex.yy.cpp COMPILE_FLAGS --header-file=${GENERATED_INCL_DIR}/lex.yy.hpp)

BISON_TARGET(SPLC_Parser src/syntax.y ${GENERATED_SRC_DIR}/syntax.tab.cpp COMPILE_FLAGS DEFINES_FILE ${GENERATED_INCL_DIR}/syntax.tab.hpp)

list(APPEND SRC_FILES ${FLEX_SPLC_Lexer_OUTPUTS} ${BISON_SPLC_Parser_OUTPUTS})

add_executable(splc ${SRC_FILES})

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)