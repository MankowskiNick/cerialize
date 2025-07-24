
#include "../helpers/test_utils.h"
#include <stdio.h>
#include <string.h>
#include "cerealize.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
    double expected; // For positive cases
} number_test_case_t;

int run_number_test(const number_test_case_t* tc) {
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
        printf("  FAIL: Should parse number without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_NUMBER) {
        printf("  FAIL: Root type should be JSON_NUMBER (got %d)\n", result.root.type);
        pass = 0;
    }
    double diff = result.root.value.number - tc->expected;
    if (!(diff < EPSILON && diff > -EPSILON)) {
        printf("  FAIL: Root number value mismatch (got %f, expected %f)\n", result.root.value.number, tc->expected);
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_number_tests() {
    number_test_case_t number_tests[] = {
        // Positive cases
        {"3.14", 0, NULL, 3.14},
        {"0", 0, NULL, 0.0},
        {"-42.5", 0, NULL, -42.5},
        {"1e10", 0, NULL, 1e10},
        {"-1e-10", 0, NULL, -1e-10},
        // Negative cases
        {"3.14.15", 1, "Multiple decimal points", 0.0},
        {"--42", 1, "Multiple negative signs", 0.0},
        {"1e", 1, "Exponent missing value", 0.0},
        {"abc", 1, "Non-numeric input", 0.0},
        {"", 1, "Empty input", 0.0},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(number_tests)/sizeof(number_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    printf("\n------------------------------\n");
    printf("[Number Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_number_test(&number_tests[i]);
        if (number_tests[i].should_fail) {
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