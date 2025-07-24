#include <stdio.h>
#include <string.h>
#include "cerealize.h"
#include "../helpers/test_utils.h"

typedef struct {
    const char* input;
    int should_fail; // 1 for negative, 0 for positive
    const char* expected_error; // Optional: expected error message/type
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
    if (tc->should_fail) {
        if (!result.failure) {
            printf("    Result: FAIL (Should fail to parse, but succeeded)\n");
            pass = 0;
        } else {
            printf("    Result: PASS (Correctly failed to parse)\n");
            if (tc->expected_error) {
                printf("      Expected error: %s\n", tc->expected_error);
            }
        }
        return pass;
    }
    // Positive test logic
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
    const char *YELLOW = "\033[1;33m";
    const char *RESET = "\033[0m";

    size_t total = sizeof(object_tests)/sizeof(object_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    printf("\n[Object Tests]\n");
    printf("| %-3s | %-40s | %-10s | %-10s |\n", "#", "Input", "Result", "Status");
    printf("|-----|------------------------------------------|------------|------------|\n");
    for (size_t i = 0; i < total; ++i) {
        const object_test_case_t *tc = &object_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = parse_json(tc->input, size);
        int pass = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        char input_display[40];
        if (tc->should_fail) {
            if (!result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Parsed");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "Error");
            }
        } else {
            if (result.failure) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Error");
            } else if (result.root.type != JSON_OBJECT || result.root.value.nodes == NULL) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
                strcpy(result_str, "Type Error");
            } else {
                strcpy(status, "PASS");
                color = GREEN;
                strcpy(result_str, "Object");
            }
        }
        snprintf(input_display, sizeof(input_display), "%s", tc->input);
        for (size_t j = 0; j < strlen(input_display); ++j) {
            if (input_display[j] == '\n' || input_display[j] == '\r' || input_display[j] == '\t') {
                input_display[j] = ' ';
            }
        }
        if (strlen(tc->input) > 40) {
            input_display[37] = '.';
            input_display[38] = '.';
            input_display[39] = '.';
            input_display[40] = '\0';
        }
        printf("| %-3zu | %-40s | %-10s | %s%-10s%s |\n", i+1, input_display, result_str, color, status, RESET);
        if (tc->should_fail) {
            if (pass) ++negative_passed; else ++negative_failed;
        } else {
            if (pass) ++positive_passed; else ++positive_failed;
        }
    }
    printf("|-----|------------------------------------------|------------|------------|\n");
    printf("  Positive: %d passed, %d failed\n", positive_passed, positive_failed);
    printf("  Negative: %d passed, %d failed\n", negative_passed, negative_failed);
    printf("  Total: %zu\n", total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}