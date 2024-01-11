add_library(SPLCCodeGen SHARED
    ASTDispatch.cc
    ObjBuilder.cc
)
target_include_directories(SPLCCodeGen PUBLIC ${SPLC_INCL_DIR})
set_target_properties(SPLCCodeGen PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB SPLCCodeGen_HEADER_FILES "${SPLC_INCL_DIR}/CodeGen/*.hh")
set_target_properties(SPLCCodeGen PROPERTIES 
    PUBLIC_HEADER "${SPLCCodeGen_HEADER_FILES}")

target_link_libraries(SPLCCodeGen SPLCCore SPLCBasic SPLCAST ${llvm_libraries})