#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
    const char* expected; // For positive cases
} string_test_case_t;

int run_string_test(const string_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("    Test: '%s'\n", tc->input);
    int pass = 1;
    if (tc->should_fail) {
        if (!result.failure) {
            printf("  FAIL: Should fail to parse, but succeeded\n");
            pass = 0;
        } else {
            printf("  PASS: Correctly failed to parse\n");
            if (tc->expected_error) {
                printf("    Expected error: %s\n", tc->expected_error);
            }
        }
        return pass;
    }
    // Positive test logic
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
        // Positive cases
        {"'test'", 0, NULL, "test"},
        {"'abc'", 0, NULL, "abc"},
        {"'hello world'", 0, NULL, "hello world"},
        {"'123'", 0, NULL, "123"},
        {"'!@#$%^&*()'", 0, NULL, "!@#$%^&*()"},
        // Negative cases
        {"test'", 1, "Missing opening quote", NULL},
        {"'test", 1, "Missing closing quote", NULL},
        {"''", 1, "Empty string not allowed", NULL},
        {"'abc", 1, "Unclosed string", NULL},
        {"'abc\n'", 1, "Newline in string not allowed", NULL},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(string_tests)/sizeof(string_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    printf("\n------------------------------\n");
    printf("[String Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_string_test(&string_tests[i]);
        if (string_tests[i].should_fail) {
            if (res) ++negative_passed; else ++negative_failed;
        } else {
            if (res) ++positive_passed; else ++positive_failed;
        }
    }
    printf("  Positive: %d passed, %d failed\n", positive_passed, positive_failed);
    printf("  Negative: %d passed, %d failed\n", negative_passed, negative_failed);
    printf("  Total: %zu\n", total);
    printf("------------------------------\n");
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}