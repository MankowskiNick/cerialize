#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    const char* expected;
} string_test_case_t;

int run_string_test(const string_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse string '%s'...\n", tc->input);
    int pass = 1;
    if (result.failure) {
        printf("  FAIL: Should parse string without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_STRING) {
        printf("  FAIL: Root type should be JSON_STRING (got %d)\n", result.root.type);
        pass = 0;
    }
    if (!(result.root.value.string && strcmp(result.root.value.string, tc->expected) == 0)) {
        printf("  FAIL: Root string value mismatch (got '%s', expected '%s')\n", result.root.value.string ? result.root.value.string : "NULL", tc->expected);
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_string_tests() {
    string_test_case_t string_tests[] = {
        {"'test'", "test"},
        {"'abc'", "abc"},
        {"'hello world'", "hello world"},
        {"'123'", "123"},
        {"'!@#$%^&*()'", "!@#$%^&*()"},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(string_tests)/sizeof(string_tests[0]);
    for (size_t i = 0; i < total; ++i) {
        int res = run_string_test(&string_tests[i]);
        if (res) ++passed; else ++failed;
    }
    printf("\nString Test Results: %d passed, %d failed, %zu total\n", passed, failed, total);
    test_summary_t summary = {passed, failed, total};
    return summary;
}