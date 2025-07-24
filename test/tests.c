#include <stdio.h>

#include "cerealize.h"

int main() {
    // char* str = "\'test\'";
    char* str = "3.14";
    // char* str = "null";
    // char* str = "true";
    // char* str = "false";
    // char* str = "{'example':{'name':'example','value': 42,'nested': {'key': 'value'}}}";
    cereal_size_t size = 69;
    json result = parse_json(str, size);
    printf("Testing JSON string: %s\n", str);
    if (result.failure) {
        printf("Failed to parse JSON: %s\n", result.error_text);
    } else {
        printf("Parsed JSON successfully.\n");
    }
    printf("JSON root type: %d\n", result.root.type);
    printf("JSON root string: %s\n", result.root.value.string ? result.root.value.string : "NULL");
    printf("JSON root number: %f\n", result.root.value.number);
    printf("JSON root boolean: %d\n", result.root.value.boolean);
    printf("JSON root is_null: %d\n", result.root.value.is_null);
    printf("JSON root nodes: %p\n", (void*)result.root.value.nodes);
    printf("JSON error text: %s\n", result.error_text);
    printf("JSON error length: %u\n", result.error_length);
    printf("JSON failure: %d\n", result.failure);
    return 0;
}