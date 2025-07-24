#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    const char* expected;
} string_test_case_t;

void run_string_test(const string_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse string '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse string without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_STRING, "Root type should be JSON_STRING", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.string && strcmp(result.root.value.string, tc->expected) == 0, "Root string value mismatch", "%s", result.root.value.string ? result.root.value.string : "NULL");
}

void run_string_tests() {
    string_test_case_t string_tests[] = {
        {"'test'", "test"},
        {"'abc'", "abc"},
        {"'hello world'", "hello world"},
        {"'123'", "123"},
        {"'!@#$%^&*()'", "!@#$%^&*()"},
    };
    for (size_t i = 0; i < sizeof(string_tests)/sizeof(string_tests[0]); ++i) {
        run_string_test(&string_tests[i]);
    }
}