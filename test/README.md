
# Test Suite Documentation

This directory contains the test suite for the cerealize project, which is focused on JSON serialization and deserialization in C. The tests verify both the parsing (deserialization) and generation (serialization) of JSON data. The suite is organized into separate files based on the type of data being tested. Below is an overview of the structure and contents of the test suite.

## Project Structure

The project is organized as follows:

- **include/**: Contains public headers for the cerealize library.
  - `cerealize.h`: Main header for JSON serialization/deserialization logic.

- **test/**: Contains the test suite and related files.
  - **cases/**: Individual test case files for different data types and features.
    - `test_bool.h`: Test cases for boolean parsing.
    - `test_null.h`: Test cases for null value parsing.
    - `test_number.h`: Test cases for number parsing.
    - `test_object.h`: Test cases for object parsing.
    - `test_string.h`: Test cases for string parsing.
    - `test_list.h`: Test cases for list parsing.
    - `test_serialize.h`: Test cases for general serialization output.
  - **helpers/**: Utility functions for running tests.
    - `test_utils.h` / `test_utils.c`: Common assertions and setup tasks.
    - `test_output_helper.h` / `test_output_helper.c`: Output formatting for test results.
  - `tests.c`: The main entry point for running all tests. Orchestrates execution of all test cases.
  - `README.md`: This documentation file.

- **build/**: (Generated) Build artifacts and CMake files.
  - Contains compiled objects, CMake cache, and build scripts.

- **CMakeLists.txt**: CMake build configuration for the project.
- **README.md**: Project-level documentation.

## Running the Tests

The recommended way to build and run the tests is to use the provided `run_tests.sh` script from the project root:

```
./run_tests.sh
```

This script will clean, configure, build, and run the test suite using CMake. The test binary will be built as `build/tests` and executed automatically.

Alternatively, you can manually build and run the tests using CMake:

```
mkdir -p build
cd build
cmake ..
make
./tests
```

## Dependencies

Ensure that you have the necessary development tools installed, including a C compiler (such as GCC) and any libraries required by the cerealize project. No external JSON libraries are required, as serialization and deserialization are implemented in C.

## Contribution

If you wish to add new tests or modify existing ones, please follow the structure outlined above and ensure that your tests are well-documented. If you add tests for serialization, consider checking both the output JSON string and the ability to round-trip (deserialize, then serialize, and compare results).