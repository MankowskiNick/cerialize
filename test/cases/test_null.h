#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
} null_test_case_t;

void run_null_test(const null_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse null '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse null without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_NULL, "Root type should be JSON_NULL", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.is_null, "Root is_null should be true", "%d", result.root.value.is_null);
}

void run_null_tests() {
    null_test_case_t null_tests[] = {
        {"null"},
        {"  null  "},
        {"\tnull\n"},
    };
    for (size_t i = 0; i < sizeof(null_tests)/sizeof(null_tests[0]); ++i) {
        run_null_test(&null_tests[i]);
    }
}