
#include <stdio.h>
#include "../helpers/test_output_helper.h"
#include "cases/test_bool.h"
#include "cases/test_null.h"
#include "cases/test_number.h"
#include "cases/test_object.h"
#include "cases/test_string.h"
#include "cases/test_list.h"
#include "cases/test_serialize.h"

test_row_t get_aggregate_output_row(const char *type, int passed, int failed, size_t total) {
    test_row_t row;
    snprintf(row.input_display, sizeof(row.input_display), "%s", type);
    snprintf(row.expected, sizeof(row.expected), "%d", passed);
    snprintf(row.result, sizeof(row.result), "%d", failed);
    snprintf(row.status, sizeof(row.status), "%zu", total);
    row.color = (failed == 0) ? "\033[0;32m" : "\033[1;33m"; // Green if no failures, yellow otherwise
    row.reset = "\033[0m";
    return row;
}


int main() {

    // ANSI color codes
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    // const char *YELLOW = "\033[1;33m";
    const char *CYAN = "\033[0;36m";
    const char *RESET = "\033[0m";

    printf("%sRunning cerealize tests...%s\n", CYAN, RESET);

    test_summary_t string_summary = run_string_tests();
    test_summary_t number_summary = run_number_tests();
    test_summary_t null_summary = run_null_tests();
    test_summary_t bool_summary = run_bool_tests();
    test_summary_t object_summary = run_object_tests();
    test_summary_t list_summary = run_list_tests();
    test_summary_t serialize_summary = run_serialize_tests();
    int total_passed = string_summary.passed + number_summary.passed + null_summary.passed + bool_summary.passed + object_summary.passed + list_summary.passed;
    int total_failed = string_summary.failed + number_summary.failed + null_summary.failed + bool_summary.failed + object_summary.failed + list_summary.failed;
    size_t total_tests = string_summary.total + number_summary.total + null_summary.total + bool_summary.total + object_summary.total + list_summary.total;

    total_passed += serialize_summary.passed;
    total_failed += serialize_summary.failed;
    total_tests += serialize_summary.total;

    test_row_t agg_rows[8];
    agg_rows[0] = get_aggregate_output_row("String", string_summary.passed, string_summary.failed, string_summary.total);
    agg_rows[1] = get_aggregate_output_row("Number", number_summary.passed, number_summary.failed, number_summary.total);
    agg_rows[2] = get_aggregate_output_row("Null", null_summary.passed, null_summary.failed, null_summary.total);
    agg_rows[3] = get_aggregate_output_row("Boolean", bool_summary.passed, bool_summary.failed, bool_summary.total);
    agg_rows[4] = get_aggregate_output_row("Object", object_summary.passed, object_summary.failed, object_summary.total);
    agg_rows[5] = get_aggregate_output_row("List", list_summary.passed, list_summary.failed, list_summary.total);
    agg_rows[6] = get_aggregate_output_row("Serialize", serialize_summary.passed, serialize_summary.failed, serialize_summary.total);
    agg_rows[7] = get_aggregate_output_row("Total", total_passed, total_failed, total_tests);

    const char *agg_headers[] = {"Type", "Passed", "Failed", "Total"};
    int agg_col_widths[] = {10, 8, 8, 8};
    print_test_table("Aggregate Test Results", agg_headers, 4, agg_col_widths, agg_rows, 8);

    if (total_failed == 0) {
        printf("%sAll tests passed!%s\n", GREEN, RESET);
    } else {
        printf("%sSome tests failed.%s\n", RED, RESET);
    }
    printf("All tests completed.\n");
    return 0;
}