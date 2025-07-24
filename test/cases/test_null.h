#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
} null_test_case_t;

int run_null_test(const null_test_case_t* tc) {
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
        printf("  FAIL: Should parse null without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_NULL) {
        printf("  FAIL: Root type should be JSON_NULL (got %d)\n", result.root.type);
        pass = 0;
    }
    if (!result.root.value.is_null) {
        printf("  FAIL: Root is_null should be true\n");
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_null_tests() {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";

    null_test_case_t null_tests[] = {
        // Positive cases
        {"null", 0, NULL},
        {"  null  ", 0, NULL},
        {"\tnull\n", 0, NULL},
        // Negative cases
        {"nul", 1, "Misspelled null"},
        {"NULL", 1, "Case sensitive null"},
        {"nulls", 1, "Extra characters after null"},
        {"", 1, "Empty input"},
        {"null null", 1, "Multiple nulls"},
    };
    size_t total = sizeof(null_tests)/sizeof(null_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    printf("\n[Null Tests]\n");
    printf("| %-3s | %-20s | %-20s | %-10s | %-10s |\n", "#", "Input", "Expected", "Result", "Status");
    printf("|-----|----------------------|----------------------|------------|------------|\n");
    for (size_t i = 0; i < total; ++i) {
        const null_test_case_t *tc = &null_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = parse_json(tc->input, size);
        int pass = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        if (tc->should_fail) {
            if (!result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Parsed");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "Error");
            }
        } else {
            if (result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Error");
            } else if (result.root.type != JSON_NULL || !result.root.value.is_null) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Type Error");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "null");
            }
        }
        printf("| %-3zu | %-20s | %-20s | %-10s | %s%-10s%s |\n", i+1, tc->input, tc->should_fail ? "-" : "null", result_str, color, status, RESET);
        if (tc->should_fail) {
            if (pass) ++negative_passed; else ++negative_failed;
        } else {
            if (pass) ++positive_passed; else ++positive_failed;
        }
    }
    printf("|-----|----------------------|----------------------|------------|------------|\n");
    printf("  Positive: %d passed, %d failed\n", positive_passed, positive_failed);
    printf("  Negative: %d passed, %d failed\n", negative_passed, negative_failed);
    printf("  Total: %zu\n", total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}