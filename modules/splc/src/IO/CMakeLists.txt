
# Flex lexer: SplcFlexLexer
FLEX_TARGET(SPLC_Lexer Lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/Lexer.cc)

# Bison parser: Parser
BISON_TARGET(SPLC_Parser Parser.yy ${CMAKE_CURRENT_BINARY_DIR}/Parser.cc COMPILE_FLAGS "-Wcex -Wmidrule-value" DEFINES_FILE ${GENERATED_INCL_DIR_IO}/Parser.hh)

# splc_IO shared library
add_library(splc_IO SHARED 
    ${FLEX_SPLC_Lexer_OUTPUTS} 
    ${BISON_SPLC_Parser_OUTPUTS} 
    Driver.cc
    Scanner.cc
)

target_include_directories(splc_IO PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
set_target_properties(splc_IO PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
set_target_properties(splc_IO PROPERTIES 
    PUBLIC_HEADER "${splc_IO_HEADER_FILES}")