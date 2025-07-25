#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
} object_test_case_t;

test_summary_t run_object_tests() {
    object_test_case_t object_tests[] = {
        // Positive cases
        {"{'example':{'nested':{'key':'value'},'name':'example','value':42}}", 0, NULL},
        {"{'example':{'name':'example','value':42,'nested':{'key':'value'}}}", 0, NULL},
        {"{  'example'  :  {  'nested'  :  {  'key'  :  'value'  }  ,  'name'  :  'example'  ,  'value'  :  42  }  }", 0, NULL},
        {"{\n\t'example' : {\n\t\t'nested' : {\n\t\t\t'key' : 'value'\n\t\t},\n\t\t'name' : 'example',\n\t\t'value' : 42\n\t}\n}", 0, NULL},
        {"   { 'a' : 1, 'b' : 2 }   ", 0, NULL},
        {"{\n  'x' : 10,\n\n  'y' : 20\n}\n", 0, NULL},
        {"{\t'a' : 123 ,\t'b' : 456 }", 0, NULL},
        // Negative cases
        {"{'example': { 'nested': { 'key': value }}}", 1, "Missing quotes around value"},
        {"{ 'a': }", 1, "Missing value for 'a'"},
        {"{ 'a' 1 }", 1, "Missing colon between key and value"},
        {"{ 'a': 1, 'b' }", 1, "Missing value for 'b'"},
        {"{ 'a': 1, , 'b': 2 }", 1, "Extra comma"},
        {"{ 'a': 1 'b': 2 }", 1, "Missing comma between pairs"},
        {"{ 'a': 1, 'b': 2", 1, "Unclosed object"},
    };
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";
    size_t total = sizeof(object_tests)/sizeof(object_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    test_row_t rows[sizeof(object_tests)/sizeof(object_tests[0])];
    for (size_t i = 0; i < total; ++i) {
        const object_test_case_t *tc = &object_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = deserialize_json(tc->input, size);
        int pass = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        char input_display[41];
        if (tc->should_fail) {
            if (!result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Object");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "Error");
            }
        } else {
            if (result.failure || result.root.type != JSON_OBJECT || result.root.value.object.nodes == NULL) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Error");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "Object");
            }
        }
        format_input_display(tc->input, input_display, sizeof(input_display));
        strcpy(rows[i].input_display, input_display);
        strcpy(rows[i].expected, ""); // Not used for object tests, leave empty
        strcpy(rows[i].result, result_str); // 'Object' or 'Error'
        strcpy(rows[i].status, status);    // 'PASS' or 'FAIL'
        rows[i].color = color;
        rows[i].reset = RESET;
        if (tc->should_fail) {
            if (pass) ++negative_passed; else ++negative_failed;
        } else {
            if (pass) ++positive_passed; else ++positive_failed;
        }
    }
    const char *headers[] = {"Input", "Result", "Status"};
    int col_widths[] = {40, 10, 10};
    print_test_table("Object Tests", headers, 3, col_widths, rows, total);
    print_test_summary(positive_passed, positive_failed, negative_passed, negative_failed, total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}