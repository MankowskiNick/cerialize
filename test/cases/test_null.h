#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
} null_test_case_t;

int run_null_test(const null_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("    Test: '%s'\n", tc->input);
    int pass = 1;
    if (result.failure) {
        printf("  FAIL: Should parse null without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_NULL) {
        printf("  FAIL: Root type should be JSON_NULL (got %d)\n", result.root.type);
        pass = 0;
    }
    if (!result.root.value.is_null) {
        printf("  FAIL: Root is_null should be true\n");
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_null_tests() {
    null_test_case_t null_tests[] = {
        {"null"},
        {"  null  "},
        {"\tnull\n"},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(null_tests)/sizeof(null_tests[0]);
    printf("\n------------------------------\n");
    printf("[Null Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_null_test(&null_tests[i]);
        if (res) ++passed; else ++failed;
    }
    printf("  Result: %d passed, %d failed, %zu total\n", passed, failed, total);
    printf("------------------------------\n");
    test_summary_t summary = {passed, failed, total};
    return summary;
}