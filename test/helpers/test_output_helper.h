#include <stddef.h>

void print_test_summary(int positive_passed, int positive_failed, int negative_passed, int negative_failed, size_t total);
#ifndef TEST_OUTPUT_HELPER_H
#define TEST_OUTPUT_HELPER_H

#include <stdio.h>
#include <string.h>

void print_test_row(size_t index, const char* input_display, const char* result_str, const char* status, const char* color, const char* reset, int input_width, int result_width, int status_width);
void format_input_display(const char* input, char* out, size_t out_size);

typedef struct {
    char input_display[64];
    char expected[32];
    char result[32];
    char status[16];
    const char *color;
    const char *reset;
} test_row_t;

void print_test_table(const char *title, const char *headers[], int num_headers, const int *col_widths, const test_row_t *rows, size_t num_rows);

#endif