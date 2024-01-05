# SPLCAnalysis
add_library(SPLCAnalysis SHARED
    UnusedVariable.cc
)
target_include_directories(SPLCAnalysis PUBLIC ${SPLC_INCL_DIR})
set_target_properties(SPLCAnalysis PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCAnalysis_HEADER_FILES "${SPLC_INCL_DIR}/Analysis/*.hh")
set_target_properties(SPLCAnalysis PROPERTIES
    PUBLIC_HEADER "${SPLCAnalysis_HEADER_FILES}")
target_link_libraries(SPLCAnalysis SPLCCore SPLCBasic SPLCAST)