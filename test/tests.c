#include <stdio.h>
#include "cases/test_bool.h"
#include "cases/test_null.h"
#include "cases/test_number.h"
#include "cases/test_object.h"
#include "cases/test_string.h"

int main() {
    printf("Running cerialize tests...\n");

    run_string_tests();
    run_number_tests();
    run_null_tests();
    run_bool_tests();
    run_object_tests();

    printf("\nAll tests completed.\n");
    return 0;
}