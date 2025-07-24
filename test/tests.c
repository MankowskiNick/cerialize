
#include <stdio.h>
#include "../helpers/test_output_helper.h"
#include "cases/test_bool.h"
#include "cases/test_null.h"
#include "cases/test_number.h"
#include "cases/test_object.h"
#include "cases/test_string.h"


int main() {

    // ANSI color codes
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *YELLOW = "\033[1;33m";
    const char *CYAN = "\033[0;36m";
    const char *RESET = "\033[0m";

    printf("%sRunning cerialize tests...%s\n", CYAN, RESET);

    test_summary_t string_summary = run_string_tests();
    test_summary_t number_summary = run_number_tests();
    test_summary_t null_summary = run_null_tests();
    test_summary_t bool_summary = run_bool_tests();
    test_summary_t object_summary = run_object_tests();

    int total_passed = string_summary.passed + number_summary.passed + null_summary.passed + bool_summary.passed + object_summary.passed;
    int total_failed = string_summary.failed + number_summary.failed + null_summary.failed + bool_summary.failed + object_summary.failed;
    size_t total_tests = string_summary.total + number_summary.total + null_summary.total + bool_summary.total + object_summary.total;

    printf("\n==============================\n");
    printf("[Aggregate Test Results]\n");
    printf("==============================\n");
    printf("| %-10s | %-8s | %-8s | %-8s |\n", "Type", "Passed", "Failed", "Total");
    printf("|------------|----------|----------|----------|\n");
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "String", string_summary.failed == 0 ? GREEN : YELLOW, string_summary.passed, RESET, string_summary.failed > 0 ? RED : GREEN, string_summary.failed, RESET, string_summary.total);
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "Number", number_summary.failed == 0 ? GREEN : YELLOW, number_summary.passed, RESET, number_summary.failed > 0 ? RED : GREEN, number_summary.failed, RESET, number_summary.total);
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "Null", null_summary.failed == 0 ? GREEN : YELLOW, null_summary.passed, RESET, null_summary.failed > 0 ? RED : GREEN, null_summary.failed, RESET, null_summary.total);
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "Bool", bool_summary.failed == 0 ? GREEN : YELLOW, bool_summary.passed, RESET, bool_summary.failed > 0 ? RED : GREEN, bool_summary.failed, RESET, bool_summary.total);
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "Object", object_summary.failed == 0 ? GREEN : YELLOW, object_summary.passed, RESET, object_summary.failed > 0 ? RED : GREEN, object_summary.failed, RESET, object_summary.total);
    printf("|------------|----------|----------|----------|\n");
    printf("| %-10s | %s%-8d%s | %s%-8d%s | %-8zu |\n", "TOTAL", total_failed == 0 ? GREEN : YELLOW, total_passed, RESET, total_failed > 0 ? RED : GREEN, total_failed, RESET, total_tests);
    printf("==============================\n");

    if (total_failed == 0) {
        printf("%sAll tests passed!%s\n", GREEN, RESET);
    } else {
        printf("%sSome tests failed.%s\n", RED, RESET);
    }
    printf("All tests completed.\n");
    return 0;
}