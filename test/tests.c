#include <stdio.h>
#include "cases/test_bool.h"
#include "cases/test_null.h"
#include "cases/test_number.h"
#include "cases/test_object.h"
#include "cases/test_string.h"


int main() {
    printf("Running cerialize tests...\n");

    test_summary_t string_summary = run_string_tests();
    test_summary_t number_summary = run_number_tests();
    test_summary_t null_summary = run_null_tests();
    test_summary_t bool_summary = run_bool_tests();
    test_summary_t object_summary = run_object_tests();

    int total_passed = string_summary.passed + number_summary.passed + null_summary.passed + bool_summary.passed + object_summary.passed;
    int total_failed = string_summary.failed + number_summary.failed + null_summary.failed + bool_summary.failed + object_summary.failed;
    size_t total_tests = string_summary.total + number_summary.total + null_summary.total + bool_summary.total + object_summary.total;

    printf("\n==============================\n");
    printf("Aggregate Test Results: %d passed, %d failed, %zu total\n", total_passed, total_failed, total_tests);
    printf("==============================\n");
    printf("\nAll tests completed.\n");
    return 0;
}