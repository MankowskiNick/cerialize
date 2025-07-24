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
