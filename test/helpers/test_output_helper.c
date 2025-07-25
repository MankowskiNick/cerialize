#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

void print_test_summary(int positive_passed, int positive_failed, int negative_passed, int negative_failed, size_t total) {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";
    printf("  Positive: %s%d passed%s, %s%d failed%s\n",
        GREEN, positive_passed, RESET,
        positive_failed > 0 ? RED : GREEN, positive_failed, RESET);
    printf("  Negative: %s%d passed%s, %s%d failed%s\n",
        GREEN, negative_passed, RESET,
        negative_failed > 0 ? RED : GREEN, negative_failed, RESET);
    printf("  Total: %zu\n", total);
}
#include "test_output_helper.h"
#include <stdio.h>
#include <string.h>

void print_test_row(size_t index, const char* input_display, const char* result_str, const char* status, const char* color, const char* reset, int input_width, int result_width, int status_width) {
    printf("| %-3zu | %-*s | %-*s | %s%-*s%s |\n",
        index,
        input_width, input_display,
        result_width, result_str,
        color, status_width, status, reset);
}

void format_input_display(const char* input, char* out, size_t out_size) {
    snprintf(out, out_size, "%s", input);
    size_t len = strlen(out);
    for (size_t j = 0; j < len; ++j) {
        if (out[j] == '\n' || out[j] == '\r' || out[j] == '\t') {
            out[j] = ' ';
        }
    }
    if (len >= out_size) len = out_size - 1;
    if (len > out_size - 1) len = out_size - 1;
    if (len > 40) {
        out[37] = '.';
        out[38] = '.';
        out[39] = '.';
        out[40] = '\0';
    }
}

void print_test_table(const char *title, const char *headers[], int num_headers, const int *col_widths, const test_row_t *rows, size_t num_rows) {
    // Make title bold and magenta
    const char *BOLD_MAGENTA = "\033[1;35m";
    const char *CYAN = "\033[0;36m";
    const char *RESET = "\033[0m";
    printf("\n%s[%s]%s\n", BOLD_MAGENTA, title, RESET);

    // Print header row with cyan borders
    printf("%s|%s", CYAN, RESET);
    for (int i = 0; i < 5; ++i) { printf("%s-%s", CYAN, RESET); }
    for (int h = 0; h < num_headers; ++h) {
        printf("%s|%s", CYAN, RESET);
        for (int i = 1; i < col_widths[h] + 3; i++) { printf("%s-%s", CYAN, RESET); }
    }
    printf("%s|%s\n", CYAN, RESET);

    // Make header row and test # bold and blue
    const char *BOLD_BLUE = "\033[1;34m";
    printf("%s|%s %s%-3s%s ", CYAN, RESET, BOLD_BLUE, "#", RESET);
    for (int h = 0; h < num_headers; ++h) {
        printf("%s|%s %s%-*s%s ", CYAN, RESET, BOLD_BLUE, col_widths[h], headers[h], RESET);
    }
    printf("%s|%s\n", CYAN, RESET);

    // Print separator row (border) dynamically sized
    printf("%s|%s", CYAN, RESET);
    for (int i = 0; i < 5; ++i) { printf("%s-%s", CYAN, RESET); }
    for (int h = 0; h < num_headers; ++h) {
        printf("%s|%s", CYAN, RESET);
        for (int i = 1; i < col_widths[h] + 3; i++) { printf("%s-%s", CYAN, RESET); }
    }
    printf("%s|%s\n", CYAN, RESET);

    for (size_t i = 0; i < num_rows; ++i) {
        printf("%s|%s %s%-3zu%s ", CYAN, RESET, BOLD_BLUE, i+1, RESET);
        printf("%s|%s %-*s ", CYAN, RESET, col_widths[0], rows[i].input_display);
        if (num_headers == 2) {
            printf("%s|%s %-*s ", CYAN, RESET, col_widths[1], rows[i].result);
        } else if (num_headers == 3) {
            // For 3-column tables, print result in col 2, status in col 3
            char result_buf[64];
            strncpy(result_buf, rows[i].result, sizeof(result_buf)-1);
            result_buf[sizeof(result_buf)-1] = '\0';
            int res_len = (int)strlen(result_buf);
            if (res_len > col_widths[1]) {
                // Truncate and add ellipsis
                if (col_widths[1] > 3) {
                    strncpy(result_buf, rows[i].result, col_widths[1]-3);
                    result_buf[col_widths[1]-3] = '.';
                    result_buf[col_widths[1]-2] = '.';
                    result_buf[col_widths[1]-1] = '.';
                    result_buf[col_widths[1]] = '\0';
                } else {
                    strncpy(result_buf, rows[i].result, col_widths[1]);
                    result_buf[col_widths[1]] = '\0';
                }
            }
            printf("%s|%s %-*s ", CYAN, RESET, col_widths[1], result_buf);
            printf("%s|%s %s%-*s%s ", CYAN, RESET, rows[i].color, col_widths[2], rows[i].status, rows[i].reset);
        } else if (num_headers > 3) {
            // For 4-column tables, print input_display, expected, result, status only
            char expected_buf[64];
            strncpy(expected_buf, rows[i].expected, sizeof(expected_buf)-1);
            expected_buf[sizeof(expected_buf)-1] = '\0';
            printf("%s|%s %-*s ", CYAN, RESET, col_widths[1], expected_buf);

            char result_buf[64];
            strncpy(result_buf, rows[i].result, sizeof(result_buf)-1);
            result_buf[sizeof(result_buf)-1] = '\0';
            int res_len = (int)strlen(result_buf);
            if (res_len > col_widths[2]) {
                if (col_widths[2] > 3) {
                    strncpy(result_buf, rows[i].result, col_widths[2]-3);
                    result_buf[col_widths[2]-3] = '.';
                    result_buf[col_widths[2]-2] = '.';
                    result_buf[col_widths[2]-1] = '.';
                    result_buf[col_widths[2]] = '\0';
                } else {
                    strncpy(result_buf, rows[i].result, col_widths[2]);
                    result_buf[col_widths[2]] = '\0';
                }
            }
            printf("%s|%s %-*s ", CYAN, RESET, col_widths[2], result_buf);
            printf("%s|%s %s%-*s%s ", CYAN, RESET, rows[i].color, col_widths[3], rows[i].status, rows[i].reset);
        }
        // Do NOT print status column again for 4-column tables
        printf("%s|%s\n", CYAN, RESET);
    }
    // Print closing separator row (border) dynamically sized
    printf("%s|%s", CYAN, RESET);
    for (int i = 0; i < 5; ++i) { printf("%s-%s", CYAN, RESET); }
    for (int h = 0; h < num_headers; ++h) {
        printf("%s|%s", CYAN, RESET);
        for (int i = 1; i < col_widths[h] + 3; i++) { printf("%s-%s", CYAN, RESET); }
    }
    printf("%s|%s\n", CYAN, RESET);
}
