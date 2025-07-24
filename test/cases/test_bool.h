#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
    int expected; // For positive cases
} bool_test_case_t;

int run_bool_test(const bool_test_case_t* tc) {
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
        printf("  FAIL: Should parse bool without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_BOOL) {
        printf("  FAIL: Root type should be JSON_BOOL (got %d)\n", result.root.type);
        pass = 0;
    }
    if (result.root.value.boolean != tc->expected) {
        printf("  FAIL: Root boolean value mismatch (got %d, expected %d)\n", result.root.value.boolean, tc->expected);
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_bool_tests() {
    bool_test_case_t bool_tests[] = {
        // Positive cases
        {"true", 0, NULL, 1},
        {"false", 0, NULL, 0},
        // Negative cases
        {"True", 1, "Case sensitive true", 0},
        {"False", 1, "Case sensitive false", 0},
        {"tru", 1, "Misspelled true", 0},
        {"fals", 1, "Misspelled false", 0},
        {"", 1, "Empty input", 0},
        {"true false", 1, "Multiple bools", 0},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(bool_tests)/sizeof(bool_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    printf("\n------------------------------\n");
    printf("[Bool Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_bool_test(&bool_tests[i]);
        if (bool_tests[i].should_fail) {
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