#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
    const char* expected; // For positive cases
} string_test_case_t;

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
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";
    size_t total = sizeof(string_tests)/sizeof(string_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    test_row_t rows[sizeof(string_tests)/sizeof(string_tests[0])];
    for (size_t i = 0; i < total; ++i) {
        const string_test_case_t *tc = &string_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = parse_json(tc->input, size);
        int pass = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        char input_display[21];
        char expected_str[32];
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
            } else if (result.root.type != JSON_STRING || !(result.root.value.string && strcmp(result.root.value.string, tc->expected) == 0)) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                snprintf(result_str, sizeof(result_str), "%s", result.root.value.string ? result.root.value.string : "NULL");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                snprintf(result_str, sizeof(result_str), "%s", result.root.value.string);
            }
        }
        format_input_display(tc->input, input_display, sizeof(input_display));
        if (!tc->should_fail)
            snprintf(expected_str, sizeof(expected_str), "%s", tc->expected);
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
    print_test_table("String Tests", headers, 4, col_widths, rows, total);
    print_test_summary(positive_passed, positive_failed, negative_passed, negative_failed, total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}