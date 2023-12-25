# splc_IR shared library
add_library(splc_IR SHARED 
    IR.cc
    IRBuilder.cc
)

target_include_directories(splc_IR PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
set_target_properties(splc_IR PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
set_target_properties(splc_IR PROPERTIES 
    PUBLIC_HEADER "${splc_IR_HEADER_FILES}")
    
target_link_libraries(splc_IR splc_Core splc_Basic splc_AST)