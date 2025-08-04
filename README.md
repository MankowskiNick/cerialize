# cerialize

[![Tests](https://github.com/MankowskiNick/cerialize/actions/workflows/test.yml/badge.svg)](https://github.com/MankowskiNick/cerialize/actions/workflows/test.yml)

**cerialize** is a lightweight, single-header JSON deserializer written in C. It provides a simple API for parsing JSON strings into C data structures, with robust error handling and support for core JSON types.

## Features

- **Single-header library**: Just include `cerialize.h` in your project.
- **Supports core JSON types**: Object, String, Number, Boolean, Null, List (Array).
- **Strict error handling**: Detailed error messages for invalid input.
- **Test suite included**: Comprehensive tests for all supported types.

---

## Getting Started

### 1. Add to Your Project

Copy `include/cerialize.h` into your project, or reference it directly.

```c
#include "cerialize.h"
```

### 2. Usage Example

```c
const char* json_str = "{\"key\": \"value\", \"num\": 42, \"flag\": true, \"missing\": null}";
json result = deserialize_json(json_str, strlen(json_str));
if (result.failure) {
    printf("Parse error: %s\n", result.error_text);
    json_free(&result);  // Clean up even on failure
    return 1;
} else {
    // Access result.root (json_object)
    // See Data Types section below
}

// Always free memory when done
json_free(&result);
```

---

## Data Types


### Main Types

- **json_type**: Enum for type discrimination.
  - `JSON_OBJECT`, `JSON_STRING`, `JSON_NUMBER`, `JSON_BOOL`, `JSON_NULL`, `JSON_LIST`
- **json_value**: Union holding the actual value.
  - `string`: `char*`
  - `number`: `float`
  - `boolean`: `bool_t` (char, TRUE/FALSE)
  - `is_null`: `bool_t`
  - `nodes`: Array of `json_node` (for objects and lists)
  - `node_count`: Number of nodes (for objects and lists)
- **json_node**: Represents a key-value pair in an object, or an element in a list.
  - For objects: `key` is set, `value` is the value.
  - For lists: `key` is NULL, `value` is the element value.
- **json_object**: Main parsed value.
  - `type`: `json_type`
  - `value`: `json_value`
- **json**: Top-level result.
  - `root`: `json_object`
  - `error_text`: Error message (if any)
  - `failure`: TRUE if parsing failed

### Example: Traversing an Object

```c
if (result.root.type == JSON_OBJECT) {
    for (cereal_size_t i = 0; i < result.root.value.node_count; ++i) {
        json_node node = result.root.value.nodes[i];
        printf("Key: %s\n", node.key);
        // Check node.value type and access accordingly
    }
}
```

### Example: Traversing a List

```c
if (result.root.type == JSON_LIST) {
    for (cereal_size_t i = 0; i < result.root.value.node_count; ++i) {
        json_node node = result.root.value.nodes[i];
        // node.key will be NULL
        // node.value holds the element value
    }
}
```

---

## Compilation & Running Tests


### Build (CMake)

Use CMake to build the test suite:

```bash
rm -rf build
mkdir -p build
cd build
cmake ..
make
cd ..
```

This will compile the test suite and place the binary in `build/tests`.

### Run Tests

You can run the tests using the provided script:

```bash
./run_tests.sh
```

Or, run the test binary directly after building:

```bash
./build/tests
```

#### Manual Compilation (Optional)

You can also compile manually:

```bash
gcc -std=c99 -Wall -Wextra -Iinclude -Itest/helpers -Itest/cases -o build/tests test/tests.c test/helpers/test_output_helper.c -g
./build/tests
```

### Test Suite Structure

- **test/cases/**: Individual test files for each data type.
- **test/helpers/**: Utility functions for testing.
- **test/tests.c**: Main test runner.

---

## Supported JSON Types

- **String**: Must be enclosed in single quotes (`\"example\"`). Newlines inside strings are not allowed.
- **Number**: Supports integers, floats, and exponents. Strict format checking.
- **Boolean**: Only accepts `true` or `false` (case-sensitive). `1` and `0` are rejected.
- **Null**: Only accepts `null` (case-sensitive).
- **Object**: Key-value pairs, keys must be strings.
- **List (Array)**: Elements separated by commas, supports mixed types. Trailing commas are allowed and ignored. Empty lists are supported.

---

## Memory Management

**Important**: Always call `json_free()` to prevent memory leaks.

### Functions

- **`json_free(json* j)`**: Frees all memory associated with a JSON structure
- **`json_object_free(json_object* obj)`**: Frees memory for individual JSON objects (used internally)

### Features

- **Recursive cleanup**: Automatically frees nested objects, arrays, and strings
- **NULL safety**: Safe to call on NULL pointers
- **Double-free protection**: Safe to call multiple times on the same structure
- **Complete reset**: Resets the structure after freeing

### Usage Examples

#### Basic Usage
```c
json result = deserialize_json(json_string, strlen(json_string));
// Use the result...
json_free(&result);  // Always free when done
```

#### Error Handling
```c
json result = deserialize_json(json_string, strlen(json_string));
if (result.failure) {
    printf("Error: %s\n", result.error_text);
    json_free(&result);  // Free even on failure
    return 1;
}
// Normal processing...
json_free(&result);
```

#### Complex Structures
```c
// Even deeply nested structures are cleaned up with one call
const char* complex = "{\"users\":[{\"name\":\"John\",\"data\":{\"age\":30}}]}";
json result = deserialize_json(complex, strlen(complex));
// Use the result...
json_free(&result);  // Frees everything recursively
```

---

## Error Handling

All parsing errors are reported via the `error_text` field in the `json` struct. Common errors include:

- Missing or mismatched quotes
- Invalid number format
- Unexpected characters
- Multiple values where only one is allowed

---

## Contribution

- Add new tests in `test/cases/`
- Use the provided helpers for assertions
- Document new features and types

---


## License

This project is licensed under the MIT License. You are free to use, modify, and distribute this software with attribution. See the LICENSE file for full details.

---

**For more details, see the code in `include/cerialize.h` and the test suite in `test/`.**
