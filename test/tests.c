#include <stdio.h>

#include "cerealize.h"

int main() {

    char* str = "{'example':{'name':'example','value': 42,'nested': {'key': 'value'}}}";
    printf("Testing JSON string: %s\n", str);
}