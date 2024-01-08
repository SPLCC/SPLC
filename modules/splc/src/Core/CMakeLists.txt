add_library(SPLCCore SHARED
    Internal.cc
    Options.cc
    System.cc
    Utils.cc
    Utils/CommandLineParser.cc
    Utils/Logging.cc
)
target_include_directories(SPLCCore PUBLIC ${SPLC_INCL_DIR})
set_target_properties(SPLCCore PROPERTIES 
    LIBRARY_OUTPUT_DIRECTORY ${GENERATED_LIB_DIR})
file(GLOB_RECURSE SPLCCore_HEADER_FILES "${SPLC_INCL_DIR}/Core/*.hh")
set_target_properties(SPLCCore PROPERTIES 
    PUBLIC_HEADER "${SPLCCore_HEADER_FILES}")