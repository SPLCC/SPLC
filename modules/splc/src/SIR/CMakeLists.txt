# SPLCSIR shared library
add_library(SPLCSIR SHARED 
    IR.cc
    IRBuilder.cc
    IROptimizer.cc
)

target_include_directories(SPLCSIR PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
set_target_properties(SPLCSIR PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCAST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
set_target_properties(SPLCSIR PROPERTIES 
    PUBLIC_HEADER "${SPLCSIR_HEADER_FILES}")
    
target_link_libraries(SPLCSIR SPLCCore SPLCBasic SPLCAST)