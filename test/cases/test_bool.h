#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int expected;
} bool_test_case_t;

int run_bool_test(const bool_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("    Test: '%s'\n", tc->input);
    int pass = 1;
    if (result.failure) {
        printf("  FAIL: Should parse bool without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_BOOL) {
        printf("  FAIL: Root type should be JSON_BOOL (got %d)\n", result.root.type);
        pass = 0;
    }
    if (result.root.value.boolean != tc->expected) {
        printf("  FAIL: Root boolean value mismatch (got %d, expected %d)\n", result.root.value.boolean, tc->expected);
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_bool_tests() {
    bool_test_case_t bool_tests[] = {
        {"true", 1},
        {"false", 0},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(bool_tests)/sizeof(bool_tests[0]);
    printf("\n------------------------------\n");
    printf("[Bool Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_bool_test(&bool_tests[i]);
        if (res) ++passed; else ++failed;
    }
    printf("  Result: %d passed, %d failed, %zu total\n", passed, failed, total);
    printf("------------------------------\n");
    test_summary_t summary = {passed, failed, total};
    return summary;
}