#ifndef TEST_MEMORY_H
#define TEST_MEMORY_H

#include "../../include/cerialize/cerialize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    const char* name;
    const char* input;
    int should_succeed; // 1 for tests that should parse successfully, 0 for parse failures
} memory_test_case_t;

test_summary_t run_memory_tests() {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";

    memory_test_case_t memory_tests[] = {
        // Basic type tests
        {"Simple String", "\"hello\"", 1},
        {"Simple Number", "42.5", 1},
        {"Simple Boolean", "true", 1},
        {"Simple Null", "null", 1},
        
        // Complex object tests
        {"Simple Object", "{\"key\":\"value\"}", 1},
        {"Nested Object", "{\"outer\":{\"inner\":\"value\"}}", 1},
        {"Object with Multiple Types", "{\"str\":\"hello\",\"num\":123,\"bool\":true,\"null\":null}", 1},
        
        // List tests
        {"Simple List", "[1,2,3]", 1},
        {"Mixed List", "[\"hello\",42,true,null]", 1},
        {"Nested List", "[[1,2],[3,4]]", 1},
        {"List of Objects", "[{\"a\":1},{\"b\":2}]", 1},
        
        // Complex nested structures
        {"Complex Nested", "{\"users\":[{\"name\":\"John\",\"age\":30},{\"name\":\"Jane\",\"age\":25}],\"count\":2}", 1},
        {"Deep Nesting", "{\"level1\":{\"level2\":{\"level3\":{\"value\":\"deep\"}}}}", 1},
        
        // Empty structures
        {"Empty Object", "{}", 1},
        {"Empty List", "[]", 1},
        
        // Error cases (should parse as failure but still test memory cleanup)
        {"Invalid JSON", "{\"key\":", 0},
        {"Unclosed String", "\"unclosed", 0},
    };
    
    size_t total = sizeof(memory_tests)/sizeof(memory_tests[0]);
    int passed = 0, failed = 0;
    test_row_t rows[sizeof(memory_tests)/sizeof(memory_tests[0])];
    
    printf("Running memory management tests...\n");
    
    for (size_t i = 0; i < total; ++i) {
        const memory_test_case_t *tc = &memory_tests[i];
        cereal_size_t size = strlen(tc->input);
        json result = deserialize_json(tc->input, size);
        
        int test_passed = 1;
        char result_str[32] = "";
        char status[16] = "";
        const char *color = GREEN;
        char input_display[41];
        
        // Check if parsing succeeded/failed as expected
        if (tc->should_succeed) {
            if (result.failure) {
                test_passed = 0;
                strcpy(result_str, "ParseFail");
            } else {
                strcpy(result_str, "Parsed");
            }
        } else {
            if (result.failure) {
                strcpy(result_str, "ExpectedFail");
            } else {
                test_passed = 0;
                strcpy(result_str, "UnexpectedPass");
            }
        }
        
        // Test memory cleanup - this is the main test
        // Call json_free and verify it doesn't crash
        json_free(&result);
        
        // After freeing, the structure should be reset
        if (result.error_text != NULL || result.failure != FALSE) {
            test_passed = 0;
            strcat(result_str, "+FreeFail");
        } else {
            strcat(result_str, "+FreeOK");
        }
        
        // Test double-free safety
        json_free(&result); // Should be safe to call again
        
        // Set status and color
        if (test_passed) {
            strcpy(status, "PASS");
            color = GREEN;
            passed++;
        } else {
            strcpy(status, "FAIL");
            color = RED;
            failed++;
        }
        
        format_input_display(tc->input, input_display, sizeof(input_display));
        strcpy(rows[i].input_display, input_display);
        strcpy(rows[i].expected, tc->name);
        strcpy(rows[i].result, result_str);
        strcpy(rows[i].status, status);
        rows[i].color = color;
        rows[i].reset = RESET;
    }

    const char *headers[] = {"Input", "Test Name", "Result", "Status"};
    int col_widths[] = {40, 20, 20, 10};
    print_test_table("Memory Management Tests", headers, 4, col_widths, rows, total);
    print_test_summary(passed, 0, 0, failed, total);
    
    test_summary_t summary = {passed, failed, total};
    printf("Memory management tests completed.\n");
    return summary;
}

// Additional specific test for edge cases
test_summary_t run_memory_edge_case_tests() {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";
    
    printf("Running memory edge case tests...\n");
    
    int passed = 0, failed = 0;
    int total_tests = 4;
    
    // Test 1: NULL pointer safety
    printf("Test 1: NULL pointer safety... ");
    json_free(NULL); // Should not crash
    json_object_free(NULL); // Should not crash
    printf("%sPASS%s\n", GREEN, RESET);
    passed++;
    
    // Test 2: Empty json structure
    printf("Test 2: Empty json structure... ");
    json empty_json;
    memset(&empty_json, 0, sizeof(json));
    json_free(&empty_json); // Should handle gracefully
    printf("%sPASS%s\n", GREEN, RESET);
    passed++;
    
    // Test 3: Manually created structure cleanup
    printf("Test 3: Manual structure cleanup... ");
    json manual_json;
    memset(&manual_json, 0, sizeof(json));
    manual_json.error_text = (char*)malloc(100);
    strcpy(manual_json.error_text, "test error");
    manual_json.failure = TRUE;
    manual_json.root.type = JSON_STRING;
    manual_json.root.value.string = (char*)malloc(10);
    strcpy(manual_json.root.value.string, "test");
    
    json_free(&manual_json);
    
    if (manual_json.error_text == NULL && manual_json.failure == FALSE && 
        manual_json.root.type == JSON_NULL) {
        printf("%sPASS%s\n", GREEN, RESET);
        passed++;
    } else {
        printf("%sFAIL%s\n", RED, RESET);
        failed++;
    }
    
    // Test 4: Multiple free calls
    printf("Test 4: Multiple free calls safety... ");
    json test_json = deserialize_json("{\"test\":\"value\"}", 15);
    json_free(&test_json);
    json_free(&test_json); // Should be safe
    json_free(&test_json); // Should be safe
    printf("%sPASS%s\n", GREEN, RESET);
    passed++;
    
    printf("Memory edge case tests: %d/%d passed\n", passed, total_tests);
    
    test_summary_t summary = {passed, failed, total_tests};
    return summary;
}

#endif
