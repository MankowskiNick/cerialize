#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    const char* expected;
} object_test_case_t;

int run_object_test(const object_test_case_t* tc, size_t test_num) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("  Test %zu:\n", test_num + 1);
    // Print input, indenting each line
    const char* p = tc->input;
    printf("    Input:\n");
    while (*p) {
        printf("      ");
        while (*p && *p != '\n') {
            putchar(*p);
            ++p;
        }
        putchar('\n');
        if (*p == '\n') ++p;
    }
    int pass = 1;
    if (result.failure) {
        printf("    Result: FAIL (Should parse object without failure)\n");
        pass = 0;
    }
    if (result.root.type != JSON_OBJECT) {
        printf("    Result: FAIL (Root type should be JSON_OBJECT, got %d)\n", result.root.type);
        pass = 0;
    }
    if (result.root.value.nodes == NULL) {
        printf("    Result: FAIL (Root nodes should not be NULL)\n");
        pass = 0;
    }
    if (pass) {
        printf("    Result: PASS\n");
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
    printf("\n------------------------------\n");
    printf("[Object Tests]\n");
    printf("------------------------------\n");
    for (size_t i = 0; i < total; ++i) {
        int res = run_object_test(&object_tests[i], i);
        if (res) ++passed; else ++failed;
    }
    printf("  Result: %d passed, %d failed, %zu total\n", passed, failed, total);
    printf("------------------------------\n");
    test_summary_t summary = {passed, failed, total};
    return summary;
}