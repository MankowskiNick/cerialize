#ifndef TEST_LIST_H
#define TEST_LIST_H

#include "../../include/cerealize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* input;
    int should_fail;
    const char* expected_error;
    int expected_count; // For positive cases
} list_test_case_t;

test_summary_t run_list_tests() {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";

    list_test_case_t list_tests[] = {
        // Positive cases
        {"[1,'a',true,null,{'x':2}]", 0, NULL, 5},
        {"[]", 0, NULL, 0},
        {"[1, 2,]", 0, NULL, 2},
        // Negative cases
        {"[1, 2", 1, "Expected closing square ']' for JSON list", 0},
        {"[1, 'bad\nstring']", 1, "Newline in string not allowed", 0},
    };
    size_t total = sizeof(list_tests)/sizeof(list_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    test_row_t rows[sizeof(list_tests)/sizeof(list_tests[0])];
    printf("Running list tests...\n");
    for (size_t i = 0; i < total; ++i) {
        const list_test_case_t *tc = &list_tests[i];
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
                if (tc->expected_error && !strstr(result.error_text, tc->expected_error)) {
                    pass = 0;
                }
            }
        } else {
            if (result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Error");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                snprintf(result_str, sizeof(result_str), "%u", result.root.value.node_count);
                if ((int)result.root.value.node_count != tc->expected_count) {
                    pass = 0;
                }
            }
        }
        format_input_display(tc->input, input_display, sizeof(input_display));
        if (!tc->should_fail)
            snprintf(expected_str, sizeof(expected_str), "%d", tc->expected_count);
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
        free(result.error_text);
    }

    const char *headers[] = {"Input", "Expected", "Result", "Status"};
    int col_widths[] = {20, 20, 10, 10};
    print_test_table("List Tests", headers, 4, col_widths, rows, total);
    print_test_summary(positive_passed, positive_failed, negative_passed, negative_failed, total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    printf("List tests completed.\n");
    return summary;
}

#endif
