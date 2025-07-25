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
    printf("\n[%s]\n", title);
    printf("| %-3s ", "#");
    for (int h = 0; h < num_headers; ++h) {
        printf("| %-*s ", col_widths[h], headers[h]);
    }
    printf("|\n");
    // Separator row
    printf("|");
    // For the index column (#), use 3 width plus 1 for padding (to match header/data row)
    for (int i = 0; i < 3; ++i) putchar('-');
    putchar('-'); // space after index
    for (int h = 0; h < num_headers; ++h) {
        putchar('|');
        for (int i = 0; i < col_widths[h]; ++i) putchar('-');
        putchar('-'); // space after column
    }
    printf("|\n");
    for (size_t i = 0; i < num_rows; ++i) {
        printf("| %-3zu ", i+1);
        printf("| %-*s ", col_widths[0], rows[i].input_display);
        if (num_headers > 1) printf("| %-*s ", col_widths[1], rows[i].expected);
        if (num_headers > 2) {
            char result_buf[64];
            strncpy(result_buf, rows[i].result, sizeof(result_buf)-1);
            result_buf[sizeof(result_buf)-1] = '\0';
            int res_len = (int)strlen(result_buf);
            if (res_len > col_widths[2]) {
                // Truncate and add ellipsis
                if (col_widths[2] > 3) {
                    strncpy(result_buf, rows[i].result, col_widths[2]-3);
                    result_buf[col_widths[2]-3] = '.';
                    result_buf[col_widths[2]-2] = '.';
                    result_buf[col_widths[2]-1] = '.';
                    result_buf[col_widths[2]] = '\0';
                } else {
                    // If column too small, just cut off
                    strncpy(result_buf, rows[i].result, col_widths[2]);
                    result_buf[col_widths[2]] = '\0';
                }
            }
            printf("| %-*s ", col_widths[2], result_buf);
        }
        if (num_headers > 3) printf("| %s%-*s%s ", rows[i].color, col_widths[3], rows[i].status, rows[i].reset);
        printf("|\n");
    }
    printf("|");
    for (int i = 0; i < 3; ++i) putchar('-');
    putchar('-');
    for (int h = 0; h < num_headers; ++h) {
        putchar('|');
        for (int i = 0; i < col_widths[h]; ++i) putchar('-');
        putchar('-');
    }
    printf("|\n");
}
