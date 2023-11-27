cmake_minimum_required(VERSION 3.5)
project(SPLC)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

find_package(FLEX REQUIRED)

if(NOT FLEX_FOUND)
  message(FATAL_ERROR "FATAL: Flex not found.")
endif()

find_package(BISON REQUIRED)

if(NOT BISON_FOUND)
  message(FATAL_ERROR "FATAL: Bison not found.")
endif()

set(GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(GENERATED_INCL_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR})

FLEX_TARGET(SPLC_Lexer src/lex.l ${GENERATED_SRC_DIR}/lex.yy.c COMPILE_FLAGS --header-file=${GENERATED_INCL_DIR}/lex.yy.h)

BISON_TARGET(SPLC_Parser src/syntax.y ${GENERATED_SRC_DIR}/syntax.tab.c COMPILE_FLAGS --report=all DEFINES_FILE ${GENERATED_INCL_DIR}/syntax.tab.h)

include_directories(include ${GENERATED_INCL_DIR})

aux_source_directory(src SRC_FILES)

list(APPEND SRC_FILES ${FLEX_SPLC_Lexer_OUTPUTS} ${BISON_SPLC_Parser_OUTPUTS})

set(MAIN_FILE src/main.c)

add_executable(splc ${MAIN_FILE} ${SRC_FILES})

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)