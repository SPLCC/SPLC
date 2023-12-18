cmake_minimum_required(VERSION 3.20)
project(SPLC)

find_package(FLEX 2.6.0)
find_package(BISON 3.6)

set(GENERATED_LIB_DIR ${CMAKE_BINARY_DIR}/lib)
set(GENERATED_EXEC_DIR ${CMAKE_BINARY_DIR}/bin)

set(GENERATED_INCL_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR})

set(GENERATED_INCL_DIR_IO ${CMAKE_CURRENT_BINARY_DIR}/include/IO)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR_IO})

set(GENERATED_INCL_DIR_CORE_UTILS ${CMAKE_CURRENT_BINARY_DIR}/include/Core/Utils)
file(MAKE_DIRECTORY ${GENERATED_INCL_DIR_CORE_UTILS})

set(GENERATED_SRC_DIR_IO ${CMAKE_CURRENT_BINARY_DIR}/src/IO)
file(MAKE_DIRECTORY ${GENERATED_SRC_DIR_IO})

list(APPEND SPLC_INCL_DIR include ${GENERATED_INCL_DIR})

aux_source_directory(src/IO SRC_FILES_IO)
aux_source_directory(src/Analysis SRC_FILES_ANALYSIS)
aux_source_directory(src/AST SRC_FILES_AST)
aux_source_directory(src/Translation SRC_FILES_TRANSLATION)
aux_source_directory(src/IR SRC_FILES_IR)
aux_source_directory(src/Core SRC_FILES_CORE)
aux_source_directory(src/Core/Utils SRC_FILES_CORE_UTILS)
aux_source_directory(src SRC_FILES)

set(SRC_DIR src)
set(SRC_DIR_IO src/IO)

# Flex lexer: SplcFlexLexer
FLEX_TARGET(SPLC_Lexer ${SRC_DIR_IO}/Lexer.ll ${GENERATED_SRC_DIR_IO}/Lexer.cc)
# FLEX_TARGET(SPLC_Lexer ${SRC_DIR_IO}/Lexer.ll ${GENERATED_SRC_DIR_IO}/Lexer.cc COMPILE_FLAGS DEFINES_FILE ${GENERATED_INCL_DIR_IO}/Lexer.hh)

# Bison parser: Parser
BISON_TARGET(SPLC_Parser ${SRC_DIR_IO}/Parser.yy ${GENERATED_SRC_DIR_IO}/Parser.cc COMPILE_FLAGS DEFINES_FILE ${GENERATED_INCL_DIR_IO}/Parser.hh)

# splc_IO shared library
add_library(splc_IO SHARED ${FLEX_SPLC_Lexer_OUTPUTS} ${BISON_SPLC_Parser_OUTPUTS} ${SRC_FILES_IO})
target_include_directories(splc_IO PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
set_target_properties(splc_IO PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
set_target_properties(splc_IO PROPERTIES 
    PUBLIC_HEADER "${splc_IO_HEADER_FILES}")
    
# splc_AST shared library
add_library(splc_AST SHARED ${SRC_FILES_AST})
target_include_directories(splc_AST PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_AST splc_IO)
set_target_properties(splc_AST PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/AST/*.hh")
set_target_properties(splc_AST PROPERTIES 
    PUBLIC_HEADER "${splc_AST_HEADER_FILES}")

# splc_Translation shared library
add_library(splc_Translation SHARED ${SRC_FILES_TRANSLATION})
target_include_directories(splc_Translation PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Translation splc_IO)
set_target_properties(splc_Translation PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_Translation_HEADER_FILES "${SPLC_INCL_DIR}/Translation/*.hh")
set_target_properties(splc_Translation PROPERTIES 
    PUBLIC_HEADER "${splc_Translation_HEADER_FILES}")

# splc_Analysis
add_library(splc_Analysis SHARED ${SRC_FILES_ANALYSIS})
target_include_directories(splc_Analysis PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Analysis splc_IO)
set_target_properties(splc_Analysis PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_Analysis_HEADER_FILES "${SPLC_INCL_DIR}/Analysis/*.hh")
set_target_properties(splc_Analysis PROPERTIES 
    PUBLIC_HEADER "${splc_Analysis_HEADER_FILES}")

# splc_IR shared library
add_library(splc_IR SHARED ${SRC_FILES_IR})
target_include_directories(splc_IR PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_IR splc_IO)
set_target_properties(splc_IR PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_IR_HEADER_FILES "${SPLC_INCL_DIR}/IR/*.hh")
set_target_properties(splc_IR PROPERTIES 
    PUBLIC_HEADER "${splc_IR_HEADER_FILES}")

# splc_Core shared library
add_library(splc_Core SHARED ${SRC_FILES_CORE} ${SRC_FILES_CORE_UTILS})
target_include_directories(splc_Core PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Core splc_IO)
set_target_properties(splc_Core PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB_RECURSE splc_Core_HEADER_FILES "${SPLC_INCL_DIR}/Core/*.hh")
set_target_properties(splc_Core PROPERTIES 
    PUBLIC_HEADER "${splc_Core_HEADER_FILES}")

# splc executable
add_executable(splc ${SRC_FILES})
target_link_libraries(splc splc_IO splc_AST splc_IR splc_Core)

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${GENERATED_EXEC_DIR})