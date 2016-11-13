#include <stdio.h>
#include <check.h>
#include "hex_to_base64.c"


START_TEST(test_hex_str_to_bytes)
{
    uint8_t expected[] = {0xde, 0xad, 0xbe, 0xbf};
    uint8_t *result = hex_str_to_bytes("deadbeef");
    ck_assert(result == expected);
    free(result);
}
END_TEST


int main(void) {
    return 0;
}
