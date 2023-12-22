add_library(splc_Core SHARED
    Internal.cc
    Options.cc
    System.cc
    Utils.cc
    Utils/CommandLineParser.cc
    Utils/Logging.cc
)
target_include_directories(splc_Core PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Core splc_IO_Lexer_Parser)
set_target_properties(splc_Core PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB_RECURSE splc_Core_HEADER_FILES "${SPLC_INCL_DIR}/Core/*.hh")
set_target_properties(splc_Core PROPERTIES 
    PUBLIC_HEADER "${splc_Core_HEADER_FILES}")