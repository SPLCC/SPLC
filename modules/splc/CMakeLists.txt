cmake_minimum_required(VERSION 3.20)
project(SPLC)

find_package(FLEX 2.6.0)
find_package(BISON 3.6)

set(GENERATED_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(GENERATED_EXEC_DIR ${CMAKE_BINARY_DIR}/bin)

set(GENERATED_INCL_DIR_IO ${CMAKE_CURRENT_BINARY_DIR}/include/IO)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR_IO})

set(GENERATED_INCL_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR})

set(SPLC_INCL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
list(APPEND SPLC_INCL_DIR ${GENERATED_INCL_DIR})

aux_source_directory(src SRC_FILES)
set(SRC_DIR src)

# Lexer (based on FLEX) and Parser (based on BISON)
flex_target(SPLC_Lexer src/IO/Lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/src/IO/Lexer.cc)
bison_target(SPLC_Parser src/IO/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/src/IO/Parser.cc COMPILE_FLAGS "-Wcex -Wmidrule-value" DEFINES_FILE ${GENERATED_INCL_DIR_IO}/Parser.hh)
add_flex_bison_dependency(SPLC_Lexer SPLC_Parser)

# add custom target for other libraries to depend on
add_custom_target(splc_IO_Lexer_Parser DEPENDS ${FLEX_SPLC_Lexer_OUTPUTS} ${BISON_SPLC_Parser_OUTPUTS} COMMENT "Consolidate Flex/Bison generated dependencies of target splc_IO_Lexer_Parser")

add_subdirectory(src/IO)
add_subdirectory(src/Core)
add_subdirectory(src/Basic)
add_subdirectory(src/AST)
add_subdirectory(src/Analysis)
add_subdirectory(src/CodeGen)
add_subdirectory(src/SIR)
add_subdirectory(src/Translation)

# splc executable
add_executable(splc ${SRC_FILES})
target_link_libraries(splc splc_IO splc_Core splc_AST splc_Translation splc_Analysis splc_CodeGen splc_SIR)

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${GENERATED_EXEC_DIR})