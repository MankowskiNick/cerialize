#define EPSILON 1e-6
#include <stdio.h>
#include <string.h>
#include "cerealize.h"

typedef struct {
    int passed;
    int failed;
} test_results_t;

#define ASSERT_TRUE_FMT(expr, msg, fmt, val) \
    do { \
        if (!(expr)) { \
            printf("  [FAIL] %s (got: " fmt ")\n", msg, val); \
            results.failed++; \
        } else { \
            results.passed++; \
        } \
    } while (0)
#define ASSERT_TRUE(expr, msg) \
    do { \
        if (!(expr)) { \
            printf("  [FAIL] %s\n", msg); \
            results.failed++; \
        } else { \
            results.passed++; \
        } \
    } while (0)

static test_results_t results = {0, 0};


typedef struct {
    const char* input;
    const char* expected;
} string_test_case_t;

typedef struct {
    const char* input;
    double expected;
} number_test_case_t;

typedef struct {
    const char* input;
    int expected;
} bool_test_case_t;

typedef struct {
    const char* input;
} null_test_case_t;

typedef struct {
    const char* input;
} object_test_case_t;

void run_string_test(const string_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse string '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse string without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_STRING, "Root type should be JSON_STRING", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.string && strcmp(result.root.value.string, tc->expected) == 0, "Root string value mismatch", "%s", result.root.value.string ? result.root.value.string : "NULL");
}

void run_number_test(const number_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse number '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse number without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_NUMBER, "Root type should be JSON_NUMBER", "%d", result.root.type);
    ASSERT_TRUE_FMT((result.root.value.number - tc->expected < EPSILON && result.root.value.number - tc->expected > -EPSILON), "Root number value mismatch (within epsilon)", "%f", result.root.value.number);
}

void run_null_test(const null_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse null '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse null without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_NULL, "Root type should be JSON_NULL", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.is_null, "Root is_null should be true", "%d", result.root.value.is_null);
}

void run_bool_test(const bool_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse bool '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse bool without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_BOOL, "Root type should be JSON_BOOL", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.boolean == tc->expected, "Root boolean value mismatch", "%d", result.root.value.boolean);
}

void run_object_test(const object_test_case_t* tc) {
    cereal_size_t size = strlen(tc->input);
    json result = parse_json(tc->input, size);
    printf("Test: parse object '%s'...\n", tc->input);
    ASSERT_TRUE(!result.failure, "Should parse object without failure");
    ASSERT_TRUE_FMT(result.root.type == JSON_OBJECT, "Root type should be JSON_OBJECT", "%d", result.root.type);
    ASSERT_TRUE_FMT(result.root.value.nodes != NULL, "Root nodes should not be NULL", "%p", (void*)result.root.value.nodes);
}

int main() {
    printf("Running cerialize tests...\n");

    string_test_case_t string_tests[] = {
        {"'test'", "test"},
        {"'abc'", "abc"},
    };
    for (size_t i = 0; i < sizeof(string_tests)/sizeof(string_tests[0]); ++i) {
        run_string_test(&string_tests[i]);
    }

    number_test_case_t number_tests[] = {
        {"3.14", 3.14},
        {"0", 0.0},
        {"-42.5", -42.5},
    };
    for (size_t i = 0; i < sizeof(number_tests)/sizeof(number_tests[0]); ++i) {
        run_number_test(&number_tests[i]);
    }

    null_test_case_t null_tests[] = {
        {"null"},
    };
    for (size_t i = 0; i < sizeof(null_tests)/sizeof(null_tests[0]); ++i) {
        run_null_test(&null_tests[i]);
    }

    bool_test_case_t bool_tests[] = {
        {"true", 1},
        {"false", 0},
    };
    for (size_t i = 0; i < sizeof(bool_tests)/sizeof(bool_tests[0]); ++i) {
        run_bool_test(&bool_tests[i]);
    }

    object_test_case_t object_tests[] = {
        {"{'example':{'nested':{'key':'value'},'name':'example','value':42}}"},
        {"{'example':{'name':'example','value':42,'nested':{'key':'value'}}}"},
    };
    for (size_t i = 0; i < sizeof(object_tests)/sizeof(object_tests[0]); ++i) {
        run_object_test(&object_tests[i]);
    }

    printf("\nTest results: %d passed, %d failed\n", results.passed, results.failed);
    return results.failed == 0 ? 0 : 1;
}