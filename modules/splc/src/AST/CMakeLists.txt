# splc_AST shared library
add_library(splc_AST SHARED
    AST.cc
    ASTBase.cc
    ASTContext.cc
    ASTContextManager.cc
    ASTProcess.cc
    Expr.cc
)
target_include_directories(splc_AST PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_AST splc_IO)
set_target_properties(splc_AST PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/AST/*.hh")
set_target_properties(splc_AST PROPERTIES 
    PUBLIC_HEADER "${splc_AST_HEADER_FILES}")