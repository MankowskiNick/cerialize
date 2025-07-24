
#include "../helpers/test_utils.h"
#include <stdio.h>
#include <string.h>
#include "cerealize.h"

typedef struct {
    const char* input;
    double expected;
} number_test_case_t;

int run_number_test(const number_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("    Test: '%s'\n", tc->input);
    int pass = 1;
    if (result.failure) {
        printf("  FAIL: Should parse number without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_NUMBER) {
        printf("  FAIL: Root type should be JSON_NUMBER (got %d)\n", result.root.type);
        pass = 0;
    }
    double diff = result.root.value.number - tc->expected;
    if (!(diff < EPSILON && diff > -EPSILON)) {
        printf("  FAIL: Root number value mismatch (got %f, expected %f)\n", result.root.value.number, tc->expected);
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_number_tests() {
    number_test_case_t number_tests[] = {
        {"3.14", 3.14},
        {"0", 0.0},
        {"-42.5", -42.5},
        {"1e10", 1e10},
        {"-1e-10", -1e-10},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(number_tests)/sizeof(number_tests[0]);
    printf("\n------------------------------\n");
    printf("[Number Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_number_test(&number_tests[i]);
        if (res) ++passed; else ++failed;
    }
    printf("  Result: %d passed, %d failed, %zu total\n", passed, failed, total);
    printf("------------------------------\n");
    test_summary_t summary = {passed, failed, total};
    return summary;
}