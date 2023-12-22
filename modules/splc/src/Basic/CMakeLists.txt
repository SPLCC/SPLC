add_library(splc_Basic SHARED
    TypeTraits.cc
)
target_include_directories(splc_Basic PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Basic splc_IO_Lexer_Parser)
set_target_properties(splc_Basic PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_Basic_HEADER_FILES "${SPLC_INCL_DIR}/Basic/*.hh")
set_target_properties(splc_Basic PROPERTIES
    PUBLIC_HEADER "${splc_Basic_HEADER_FILES}")

target_link_libraries(splc_Basic splc_Core)