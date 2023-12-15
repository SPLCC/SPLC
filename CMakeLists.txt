cmake_minimum_required(VERSION 3.13)

if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
")
endif()

set(CMAKE_CXX_STANDARD 20)

project(SPLCC)

# Include the subdirectories containing other CMakeLists.txt files.
add_subdirectory(modules/splc)
add_subdirectory(modules/libspl)


# Add compile options
if (MSVC)
  add_compile_options(/W4 "$<$<CONFIG:DEBUG>:/O0;/g3;/ggdb>" "$<$<CONFIG:RELEASE>:/O3>")
else()
  add_compile_options(-Wall -Wextra -Wpedantic "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>" "$<$<CONFIG:RELEASE>:-O3>")
endif()