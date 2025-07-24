#ifndef TEST_OUTPUT_HELPER_H
#define TEST_OUTPUT_HELPER_H

#include <stdio.h>
#include <string.h>

void print_test_row(size_t index, const char* input_display, const char* result_str, const char* status, const char* color, const char* reset, int input_width, int result_width, int status_width);
void format_input_display(const char* input, char* out, size_t out_size);

#endif