#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int expected;
} bool_test_case_t;

void run_bool_test(const bool_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse bool '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse bool without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_BOOL, "Root type should be JSON_BOOL", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.boolean == tc->expected, "Root boolean value mismatch", "%d", result.root.value.boolean);
}

void run_bool_tests() {
    bool_test_case_t bool_tests[] = {
        {"true", 1},
        {"false", 0},
    };
    for (size_t i = 0; i < sizeof(bool_tests)/sizeof(bool_tests[0]); ++i) {
        run_bool_test(&bool_tests[i]);
    }
}