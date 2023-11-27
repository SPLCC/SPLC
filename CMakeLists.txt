cmake_minimum_required(VERSION 3.5)

if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

project(SPLCC)

# Include the subdirectories containing other CMakeLists.txt files.
add_subdirectory(modules/splc)
add_subdirectory(modules/libspl)