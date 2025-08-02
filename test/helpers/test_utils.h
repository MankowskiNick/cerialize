#define EPSILON 1e-5
#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cerialize.h"

typedef struct { int passed, failed; size_t total; } test_summary_t;
#ifndef TEST_UTILS_H
#define TEST_UTILS_H
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cerialize.h"

#define EPSILON 1e-5
#define ASSERT_TRUE(condition, message) \
    if (!(condition)) { \
        printf("  [FAIL] %s\n", message); \
    }
#define ASSERT_TRUE_FMT(condition, message, fmt, ...) \
    if (!(condition)) { \
        printf("  [FAIL] %s: " fmt "\n", message, __VA_ARGS__); \
    }

void assert_equal_int(int expected, int actual, const char* message) {
    if (expected != actual) {
        printf("  [FAIL] %s (expected: %d, got: %d)\n", message, expected, actual);
    }
}

void assert_equal_double(double expected, double actual, const char* message) {
    if (fabs(expected - actual) > EPSILON) {
        printf("  [FAIL] %s (expected: %f, got: %f)\n", message, expected, actual);
    }
}

void assert_equal_string(const char* expected, const char* actual, const char* message) {
    if (strcmp(expected, actual) != 0) {
        printf("  [FAIL] %s (expected: '%s', got: '%s')\n", message, expected, actual);
    }
}

void assert_true(int condition, const char* message) {
    if (!condition) {
        printf("  [FAIL] %s\n", message);
    }
}

#endif
#endif // TEST_UTILS_H