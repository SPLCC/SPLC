add_library(SPLCBasic SHARED
    Type.cc    
    TypeTraits.cc
)

target_include_directories(SPLCBasic PUBLIC ${SPLC_INCL_DIR})
set_target_properties(SPLCBasic PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCBasic_HEADER_FILES "${SPLC_INCL_DIR}/Basic/*.hh")
set_target_properties(SPLCBasic PROPERTIES
    PUBLIC_HEADER "${SPLCBasic_HEADER_FILES}")

target_link_libraries(SPLCBasic SPLCCore)