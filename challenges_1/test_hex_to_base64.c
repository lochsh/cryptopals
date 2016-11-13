#include <stdio.h>
#include <stdint.h>
#include "hex_to_base64.c"


void test_hex_char_to_padded_byte() {
    char hex_char = 'd';
    uint8_t expected = 13;
    uint8_t result = hex_char_to_padded_byte(hex_char);
    if (expected == result) {
        printf("Test hex char to padded byte passes\n");
    }
}


void test_hex_char_pair_to_byte() {
    uint8_t expected = 222;
    uint8_t result = hex_char_pair_to_byte('d', 'e');
    if (expected == result) {
        printf("Test hex char pair to byte passes\n");
    }
}


void test_hex_str_to_bytes() {
    uint8_t expected[] = {0xde, 0xad, 0xbe, 0xef};
    uint8_t* result = hex_str_to_bytes("deadbeef");
    int i;
    int fail = 0;

    for (i = 0; i < 4; i++) {
        if (expected[i] != result[i]) {
            fail = 1;
        }
    }
    if (fail == 0) {
        printf("Test hex str to bytes passes\n");
    }
}


int main() {
    test_hex_char_to_padded_byte();
    test_hex_char_pair_to_byte();
    test_hex_str_to_bytes();
    return 0;
}
