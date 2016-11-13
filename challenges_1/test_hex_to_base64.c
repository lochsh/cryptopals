#include <stdio.h>
#include "../minunit.h"
#include "hex_to_base64.c"

static char* test_hex_str_to_bytes() {
    uint8_t expected[] = {0xde, 0xad, 0xbe, 0xbf};
    uint8_t *result = hex_str_to_bytes("deadbeef");
    mu_assert("error deadbeef!", result == expected);
    free(result);
    return 0;
}

int main() {
    char *result = test_hex_str_to_bytes();
    return result != 0;
 }
