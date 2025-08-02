#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"
#include <stdio.h>
#include <string.h>
#include "cerialize.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
    double expected; // For positive cases
} number_test_case_t;

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
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";
    size_t total = sizeof(number_tests)/sizeof(number_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    test_row_t rows[sizeof(number_tests)/sizeof(number_tests[0])];
    for (size_t i = 0; i < total; ++i) {
        const number_test_case_t *tc = &number_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = deserialize_json(tc->input, size);
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
            } else if (result.root.type != JSON_NUMBER) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Type Error");
            } else {
                double diff = result.root.value.number - tc->expected;
                if (!(diff < EPSILON && diff > -EPSILON)) {
                    strcpy(status, "FAIL");
                    color = RED;
                    pass = 0;
                    snprintf(result_str, sizeof(result_str), "%f", result.root.value.number);
                } else {
                    strcpy(status, "PASS");
                    color = GREEN;
                    snprintf(result_str, sizeof(result_str), "%f", result.root.value.number);
                }
            }
        }
        format_input_display(tc->input, input_display, sizeof(input_display));
        if (!tc->should_fail)
            snprintf(expected_str, sizeof(expected_str), "%f", tc->expected);
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
    int col_widths[] = {20, 20, 15, 10};
    print_test_table("Number Tests", headers, 4, col_widths, rows, total);
    print_test_summary(positive_passed, positive_failed, negative_passed, negative_failed, total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}