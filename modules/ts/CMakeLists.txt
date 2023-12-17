cmake_minimum_required(VERSION 3.20)

# =====================================
# =            Google Test
# =====================================

# Set DOWNLOAD_EXTRACT_TIMESTAMP to new as suggested, to get the project rebuilt when the downloaded content updates
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# =====================================
# =  SPL Compiler Collection:  Test
# =====================================
project(SPLCC_TEST)