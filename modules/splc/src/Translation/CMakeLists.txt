add_library(splc_Translation SHARED
    TranslationContext.cc
    TranslationContextManager.cc
    TranslationManager.cc
    TranslationUnit.cc
)
target_include_directories(splc_Translation PUBLIC ${SPLC_INCL_DIR})
add_dependencies(splc_Translation splc_AST)
set_target_properties(splc_Translation PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB splc_Translation_HEADER_FILES "${SPLC_INCL_DIR}/Translation/*.hh")
set_target_properties(splc_Translation PROPERTIES 
    PUBLIC_HEADER "${splc_Translation_HEADER_FILES}")

target_link_libraries(splc_Translation splc_Core splc_Basic splc_AST)