#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    double expected;
} number_test_case_t;

void run_number_test(const number_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse number '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse number without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_NUMBER, "Root type should be JSON_NUMBER", "%d", result.root.type);
    ASSERT_TRUE_FMT((result.root.value.number - tc->expected < EPSILON && result.root.value.number - tc->expected > -EPSILON), "Root number value mismatch (within epsilon)", "%f", result.root.value.number);
}

void run_number_tests() {
    number_test_case_t number_tests[] = {
        {"3.14", 3.14},
        {"0", 0.0},
        {"-42.5", -42.5},
        {"1e10", 1e10},
        {"-1e-10", -1e-10},
    };
    for (size_t i = 0; i < sizeof(number_tests)/sizeof(number_tests[0]); ++i) {
        run_number_test(&number_tests[i]);
    }
}