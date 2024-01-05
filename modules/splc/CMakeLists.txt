cmake_minimum_required(VERSION 3.20)
project(SPLC)

# ===================================================================
#                 Process FLEX and BISON dependencies
# ===================================================================
find_package(FLEX 2.6.0 REQUIRED)
find_package(BISON 3.6 REQUIRED)

# ===================================================================
#                      Process LLVM dependency
# ===================================================================
find_package(LLVM 18.0.0 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
# TODO: determine necessary libraries to link
llvm_map_components_to_libnames(llvm_libs support core irreader)

# ===================================================================
#                    Setup Generation Directories
# ===================================================================
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

include_directories(include ${GENERATED_INCL_DIR})

# Lexer (based on FLEX) and Parser (based on BISON)
flex_target(SPLCLexer src/IO/Lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/src/IO/Lexer.cc)
bison_target(SPLCParser src/IO/Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/src/IO/Parser.cc COMPILE_FLAGS "-Wcex -Wmidrule-value" DEFINES_FILE ${GENERATED_INCL_DIR_IO}/Parser.hh)
add_flex_bison_dependency(SPLCLexer SPLCParser)

# add custom target for other libraries to depend on
add_custom_target(SPLCIO_Lexer_Parser DEPENDS ${FLEX_SPLCLexer_OUTPUTS} ${BISON_SPLCParser_OUTPUTS} COMMENT "Consolidate Flex/Bison generated dependencies of target SPLCIO_Lexer_Parser")

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
target_link_libraries(splc SPLCIO SPLCCore SPLCAST SPLCTranslation SPLCAnalysis SPLCCodeGen SPLCSIR)

set_target_properties(splc PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${GENERATED_EXEC_DIR})