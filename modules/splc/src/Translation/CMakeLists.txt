add_library(SPLCTranslation STATIC
    TranslationContext.cc
    TranslationContextManager.cc
    TranslationManager.cc
    TranslationUnit.cc
)
target_include_directories(SPLCTranslation PUBLIC ${SPLC_INCL_DIR})
add_dependencies(SPLCTranslation SPLCAST)
set_target_properties(SPLCTranslation PROPERTIES 
    ARCHIVE_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCTranslation_HEADER_FILES "${SPLC_INCL_DIR}/Translation/*.hh")
set_target_properties(SPLCTranslation PROPERTIES 
    PUBLIC_HEADER "${SPLCTranslation_HEADER_FILES}")

target_link_libraries(SPLCTranslation SPLCCore SPLCBasic SPLCAST)