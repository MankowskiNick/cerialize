#ifndef TEST_SERIALIZE_H
#define TEST_SERIALIZE_H

#include "../../include/cerialize/cerialize.h"
#include "../helpers/test_utils.h"
#include "../helpers/test_output_helper.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Test case for serialization
typedef struct {
    json input;                // The JSON object to serialize
    const char* expected;      // The expected serialized string
    int should_fail;           // 1 if serialization should fail, 0 otherwise
    const char* expected_error;// Optional: expected error message
} serialize_test_case_t;

test_summary_t run_serialize_tests();

// Helper to build a json object for a string
static json make_json_string(const char* s) {
    json_object obj;
    obj.type = JSON_STRING;
    obj.value.string = (char*)s;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

// Helper to build a json object for a number
static json make_json_number(float n) {
    json_object obj;
    obj.type = JSON_NUMBER;
    obj.value.number = n;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

// Helper to build a json object for a bool
static json make_json_bool(int b) {
    json_object obj;
    obj.type = JSON_BOOL;
    obj.value.boolean = b ? 1 : 0;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

// Helper to build a json object for null
static json make_json_null() {
    json_object obj;
    obj.type = JSON_NULL;
    obj.value.is_null = 1;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

// Helper to build a json object for a list
static json make_json_list(json* items, size_t count) {
    json_list l;
    l.count = count;
    l.items = (json_object*)malloc(sizeof(json_object) * count);
    for (size_t i = 0; i < count; ++i) l.items[i] = items[i].root;
    json_object obj;
    obj.type = JSON_LIST;
    obj.value.list = l;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

// Helper to build a json object for an object
static json make_json_object(const char** keys, json* values, size_t count) {
    json_node* nodes = NULL;
    if (count > 0) {
        nodes = (json_node*)malloc(sizeof(json_node) * count);
        for (size_t i = 0; i < count; ++i) {
            // Deep copy key
            size_t klen = strlen(keys[i]);
            nodes[i].key = (char*)malloc(klen + 1);
            strcpy(nodes[i].key, keys[i]);
            // Deep copy value
            nodes[i].value.type = values[i].root.type;
            nodes[i].value.value = values[i].root.value;
        }
    }
    json_body body = {
        .nodes = nodes,
        .node_count = count
    };

    json_object obj;
    obj.type = JSON_OBJECT;
    obj.value.object = body;
    json j = { .root = obj, .failure = 0, .error_text = NULL };
    return j;
}

test_summary_t run_serialize_tests() {
    const char *GREEN = "\033[0;32m";
    const char *RED = "\033[0;31m";
    const char *RESET = "\033[0m";

    // Prepare test cases
    // Simple types
    serialize_test_case_t serialize_tests[] = {
        // String
        { make_json_string("abc"), "abc", 0, NULL },
        // Number
        { make_json_number(3.14f), "3.140000", 0, NULL },
        // Bool true
        { make_json_bool(1), "true", 0, NULL },
        // Bool false
        { make_json_bool(0), "false", 0, NULL },
        // Null
        { make_json_null(), "null", 0, NULL },
        // List [1,2]
        { make_json_list((json[]){ make_json_number(1), make_json_number(2) }, 2), "[1.000000,2.000000]", 0, NULL },
        // Empty list
        { make_json_list(NULL, 0), "[]", 0, NULL },
        // Object { \"a\":1, \"b\":2 }
        { make_json_object((const char*[]){ "a", "b" }, (json[]){ make_json_number(1), make_json_number(2) }, 2), "{a:1.000000,b:2.000000}", 0, NULL },
        // Empty object
        { make_json_object(NULL, NULL, 0), "{}", 0, NULL },
        // Nested object { \"x\":[true,null] }
        { make_json_object((const char*[]){ "x" }, (json[]){ make_json_list((json[]){ make_json_bool(1), make_json_null() }, 2) }, 1), "{x:[true,null]}", 0, NULL },
        // Should fail: unknown type (simulate by passing empty json)
        { { .root = { .type = 99 } }, NULL, 1, NULL },
    };
    size_t total = sizeof(serialize_tests)/sizeof(serialize_tests[0]);
    int negative_passed = 0, negative_failed = 0;
    int positive_passed = 0, positive_failed = 0;
    test_row_t rows[sizeof(serialize_tests)/sizeof(serialize_tests[0])];
    for (size_t i = 0; i < total; ++i) {
        const serialize_test_case_t *tc = &serialize_tests[i];
        char* result_str = serialize_json(&tc->input);
        int pass = 1;
        char status[16] = "";
        const char *color = GREEN;
        char input_display[32];
        char expected_str[64];
        snprintf(input_display, sizeof(input_display), "type=%d", tc->input.root.type);
        if (tc->should_fail) {
            if (result_str != NULL) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
            } else {
                strcpy(status, "PASS");
                color = GREEN;
            }
        } else {
            if (result_str == NULL) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
            } else if (strcmp(result_str, tc->expected) != 0) {
                strcpy(status, "FAIL");
                color = RED;
                pass = 0;
            } else {
                strcpy(status, "PASS");
                color = GREEN;
            }
        }
        snprintf(expected_str, sizeof(expected_str), "%s", tc->expected ? tc->expected : "-");
        strcpy(rows[i].input_display, input_display);
        strcpy(rows[i].expected, expected_str);
        strcpy(rows[i].result, result_str ? result_str : "NULL");
        strcpy(rows[i].status, status);
        rows[i].color = color;
        rows[i].reset = RESET;
        if (tc->should_fail) {
            if (pass) ++negative_passed; else ++negative_failed;
        } else {
            if (pass) ++positive_passed; else ++positive_failed;
        }
        if (result_str) free(result_str);
    }
    const char *headers[] = {"Input", "Expected", "Result", "Status"};
    int col_widths[] = {20, 20, 20, 10};
    print_test_table("Serialize Tests", headers, 4, col_widths, rows, total);
    print_test_summary(positive_passed, positive_failed, negative_passed, negative_failed, total);
    test_summary_t summary = {positive_passed + negative_passed, positive_failed + negative_failed, total};
    return summary;
}

#endif
