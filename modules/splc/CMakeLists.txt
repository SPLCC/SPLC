cmake_minimum_required(VERSION 3.5)
project(SPLC)

find_package(FLEX 2.6.0)
find_package(BISON 3.6)

set(GENERATED_INCL_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR})
include_directories(include ${GENERATED_INCL_DIR})

set(GENERATED_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR})
aux_source_directory(src SRC_FILES)
aux_source_directory(src/core SRC_FILES_CORE)
aux_source_directory(src/core/utils SRC_FILES_CORE_UTILS)
aux_source_directory(src/ir SRC_FILES_IR)

FLEX_TARGET(SPLC_lexer src/lexer.ll ${GENERATED_SRC_DIR}/lexer.cc COMPILE_FLAGS --header-file=${GENERATED_INCL_DIR}/lexer.hh)

BISON_TARGET(SPLC_parser src/parser.yy ${GENERATED_SRC_DIR}/parser.cc COMPILE_FLAGS DEFINES_FILE ${GENERATED_INCL_DIR}/parser.hh)

list(APPEND SRC_FILES ${FLEX_SPLC_lexer_OUTPUTS} ${BISON_SPLC_parser_OUTPUTS})

add_executable(splc ${SRC_FILES} ${SRC_FILES_CORE} ${SRC_FILES_CORE_UTILS} ${SRC_FILES_IR})

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)