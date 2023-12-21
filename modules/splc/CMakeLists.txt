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
set(SRC_DIR_IO src/IO)

add_subdirectory(src/IO)
add_subdirectory(src/Core)
add_subdirectory(src/Basic)
add_subdirectory(src/AST)
add_subdirectory(src/Analysis)
add_subdirectory(src/CodeGen)
add_subdirectory(src/Translation)

# splc executable
add_executable(splc ${SRC_FILES})
target_link_libraries(splc splc_IO splc_Core splc_AST splc_Translation splc_Analysis splc_CodeGen)

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${GENERATED_EXEC_DIR})