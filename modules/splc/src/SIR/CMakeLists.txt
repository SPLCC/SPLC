# # splc_SIR shared library
# add_library(splc_SIR SHARED 
#     IR.cc
#     IRBuilder.cc
#     IROptimizer.cc
# )

# target_include_directories(splc_SIR PUBLIC ${SPLC_INCL_DIR} ${GENERATED_INCL_DIR_IO})
# set_target_properties(splc_SIR PROPERTIES 
#     LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
# file(GLOB splc_AST_HEADER_FILES "${SPLC_INCL_DIR}/IO/*.hh;${GENERATED_INCL_DIR_IO}/*.hh")
# set_target_properties(splc_SIR PROPERTIES 
#     PUBLIC_HEADER "${splc_SIR_HEADER_FILES}")
    
# target_link_libraries(splc_SIR splc_Core splc_Basic splc_AST)