add_library(splc_CodeGen SHARED
    SimInstGen.cc
)
target_include_directories(splc_CodeGen PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_CodeGen splc_IO_Lexer_Parser)
set_target_properties(splc_CodeGen PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_CodeGen_HEADER_FILES "${SPLC_INCL_DIR}/CodeGen/*.hh")
set_target_properties(splc_CodeGen PROPERTIES 
    PUBLIC_HEADER "${splc_CodeGen_HEADER_FILES}")

target_link_libraries(splc_CodeGen splc_Core splc_Basic splc_AST)