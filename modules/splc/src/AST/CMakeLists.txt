# splc_AST shared library
add_library(splc_AST SHARED
    ASTBase.cc
    ASTBasePolymorphism.cc
    ASTBaseProcess.cc
    ASTBaseType.cc
    ASTBaseValue.cc
    ASTContext.cc
    ASTContextManager.cc
    ASTProcess.cc
    ASTSymbol.cc
    DerivedAST.cc
    Expr.cc
    SymbolEntry.cc
    TypeCheck.cc
)
target_include_directories(splc_AST PUBLIC ${SPLC_INCL_DIR})
set_target_properties(splc_AST PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/AST/*.hh")
set_target_properties(splc_AST PROPERTIES 
    PUBLIC_HEADER "${splc_AST_HEADER_FILES}")

target_link_libraries(splc_AST splc_Core splc_Basic)