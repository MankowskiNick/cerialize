#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    const char* expected;
} object_test_case_t;

void run_object_test(const object_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse object '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse object without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_OBJECT, "Root type should be JSON_OBJECT", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.nodes != NULL, "Root nodes should not be NULL", "%p", (void*)result.root.value.nodes);
}

void run_object_tests() {
    object_test_case_t object_tests[] = {
        {"{'example':{'nested':{'key':'value'},'name':'example','value':42}}", "expected_value_1"},
        {"{'example':{'name':'example','value':42,'nested':{'key':'value'}}}", "expected_value_2"},
    };
    for (size_t i = 0; i < sizeof(object_tests)/sizeof(object_tests[0]); ++i) {
        run_object_test(&object_tests[i]);
    }
}