#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    const char* expected;
} object_test_case_t;

int run_object_test(const object_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse object '%s'...\n", tc->input);
    int pass = 1;
    if (result.failure) {
        printf("  FAIL: Should parse object without failure\n");
        pass = 0;
    }
    if (result.root.type != JSON_OBJECT) {
        printf("  FAIL: Root type should be JSON_OBJECT (got %d)\n", result.root.type);
        pass = 0;
    }
    if (result.root.value.nodes == NULL) {
        printf("  FAIL: Root nodes should not be NULL\n");
        pass = 0;
    }
    if (pass) {
        printf("  PASS\n");
    }
    return pass;
}

test_summary_t run_object_tests() {
    object_test_case_t object_tests[] = {
        // No whitespace
        {"{'example':{'nested':{'key':'value'},'name':'example','value':42}}", "expected_value_1"},
        // Different key order
        {"{'example':{'name':'example','value':42,'nested':{'key':'value'}}}", "expected_value_2"},
        // Whitespace between keys and values
        {"{  'example'  :  {  'nested'  :  {  'key'  :  'value'  }  ,  'name'  :  'example'  ,  'value'  :  42  }  }", "expected_value_3"},
        // Newlines and tabs
        {"{\n\t'example' : {\n\t\t'nested' : {\n\t\t\t'key' : 'value'\n\t\t},\n\t\t'name' : 'example',\n\t\t'value' : 42\n\t}\n}", "expected_value_4"},
        // Whitespace before and after object
        {"   { 'a' : 1, 'b' : 2 }   ", "expected_value_5"},
        // Multiple spaces and newlines between pairs
        {"{\n  'x' : 10,\n\n  'y' : 20\n}\n", "expected_value_6"},
        // Tabs and spaces mixed
        {"{\t'a' : 123 ,\t'b' : 456 }", "expected_value_7"},
    };
    int passed = 0, failed = 0;
    size_t total = sizeof(object_tests)/sizeof(object_tests[0]);
    for (size_t i = 0; i < total; ++i) {
        int res = run_object_test(&object_tests[i]);
        if (res) ++passed; else ++failed;
    }
    printf("\nObject Test Results: %d passed, %d failed, %zu total\n", passed, failed, total);
    test_summary_t summary = {passed, failed, total};
    return summary;
}