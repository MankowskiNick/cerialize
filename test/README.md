# Test Suite Documentation

This directory contains the test suite for the cerialize project. The tests are organized into separate files based on the type of data being parsed. Below is an overview of the structure and contents of the test suite.

## Directory Structure

- **cases/**: Contains individual test case files for different data types.
  - **test_bool.c**: Test cases for boolean parsing.
  - **test_null.c**: Test cases for null value parsing.
  - **test_number.c**: Test cases for number parsing.
  - **test_object.c**: Test cases for object parsing.
  - **test_string.c**: Test cases for string parsing.

- **helpers/**: Contains utility functions that assist in running tests.
  - **test_utils.c**: Common assertions and setup tasks.

- **tests.c**: The main entry point for running all tests. This file orchestrates the execution of the tests defined in the other files.

## Running the Tests

To run the tests, compile the `tests.c` file along with the test case files and the helper utilities. Use the following command:

```
gcc -o test_suite tests.c cases/test_bool.c cases/test_null.c cases/test_number.c cases/test_object.c cases/test_string.c helpers/test_utils.c
```

After compiling, execute the test suite with:

```
./test_suite
```

## Dependencies

Ensure that you have the necessary development tools installed, including a C compiler (such as GCC) and any libraries required by the cerialize project.

## Contribution

If you wish to add new tests or modify existing ones, please follow the structure outlined above and ensure that your tests are well-documented.