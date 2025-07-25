#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
} null_test_case_t;

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
    test_row_t rows[sizeof(null_tests)/sizeof(null_tests[0])];
    for (size_t i = 0; i < total; ++i) {
        const null_test_case_t *tc = &null_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = parse_json(tc->input, size);
        int pass = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        char input_display[21];
        char expected_str[21];
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
        format_input_display(tc->input, input_display, sizeof(input_display));
        if (!tc->should_fail)
            strcpy(expected_str, "null");
        else
            strcpy(expected_str, "-");
        strcpy(rows[i].input_display, input_display);
        strcpy(rows[i].expected, expected_str);
        strcpy(rows[i].result, result_str);
        strcpy(rows[i].status, status);
        rows[i].color = color;
        rows[i].reset = RESET;
        if (tc->should_fail) {
            if (pass) ++negative_passed; else ++negative_failed;
        } else {
            if (pass) ++positive_passed; else ++positive_failed;
        }
    }
    const char *headers[] = {"Input", "Expected", "Result", "Status"};
    int col_widths[] = {20, 20, 10, 10};
    print_test_table("Null Tests", headers, 4, col_widths, rows, total);
    printf("  Positive: %d passed, %d failed\n", positive_passed, positive_failed);
    printf("  Negative: %d passed, %d failed\n", negative_passed, negative_failed);
    printf("  Total: %zu\n", total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}