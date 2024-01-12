# SPLCIO STATIC library
add_library(SPLCIO STATIC 
    ${FLEX_SPLCLexer_OUTPUTS} 
    ${BISON_SPLCParser_OUTPUTS} 
    Driver.cc
    Scanner.cc
)

target_include_directories(SPLCIO PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
add_dependencies(SPLCIO SPLCIO_Lexer_Parser)
set_target_properties(SPLCIO PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCAST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
set_target_properties(SPLCIO PROPERTIES 
    PUBLIC_HEADER "${SPLCIO_HEADER_FILES}")
    
target_link_libraries(SPLCIO SPLCAST SPLCCore)