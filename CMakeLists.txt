cmake_minimum_required(VERSION 3.10)
project(cerialize C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directory for the test binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/test/helpers
    ${CMAKE_SOURCE_DIR}/test/cases
)

# Source files for the test runner and helpers
set(TEST_SOURCES
    test/tests.c
    test/helpers/test_output_helper.c
)

# Add executable for tests
add_executable(tests ${TEST_SOURCES})

target_compile_options(tests PRIVATE -Wall -Wextra -g)
