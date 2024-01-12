# SPLCAST STATIC library
add_library(SPLCAST STATIC
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
target_include_directories(SPLCAST PUBLIC ${SPLC_INCL_DIR})
set_target_properties(SPLCAST PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCAST_HEADER_FILES "${SPLC_INCL_DIR}/AST/*.hh")
set_target_properties(SPLCAST PROPERTIES 
    PUBLIC_HEADER "${SPLCAST_HEADER_FILES}")

target_link_libraries(SPLCAST SPLCCore SPLCBasic)