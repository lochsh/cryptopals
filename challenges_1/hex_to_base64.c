#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK_4_LSB 0x0F
#define ASCII_NUM_LETTER_BOUND 64


static uint8_t hex_char_to_padded_byte(const char hex_char) {
    return (hex_char & MASK_4_LSB) + (hex_char / ASCII_NUM_LETTER_BOUND) * 9;
}


static uint8_t hex_char_pair_to_byte(const char hex_ms, const char hex_ls) {
    return (hex_char_to_padded_byte(hex_ms) << 4) +
            hex_char_to_padded_byte(hex_ls);
}


static uint8_t* hex_str_to_bytes(const char* const hex) {
    if (strlen(hex) % 2 != 0) {
        printf("Invalid hex string, must be even length\n");
        return NULL;
    }

    const size_t num_bytes = strlen(hex) / 2;
    uint8_t *bytes = (uint8_t *) malloc(num_bytes);

    if (bytes == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    for (size_t i = 0; i < num_bytes; i++) {
        bytes[i] = hex_char_pair_to_byte(hex[i*2], hex[i*2 + 1]);
    }

    return bytes;
}


static void base_64_encode(const uint8_t* const bytes, const size_t num_bytes,
                           char* const b64) {

    const char* const b64_ref = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";

    size_t i;
    for (i = 0; i < num_bytes/3; i++) {
        b64[i*4    ] = b64_ref[ (bytes[i*3    ] & 0xFC) >> 2];
        b64[i*4 + 1] = b64_ref[((bytes[i*3    ] & 0x03) << 4) +
                               ((bytes[i*3 + 1] & 0xF0) >> 4)];
        b64[i*4 + 2] = b64_ref[((bytes[i*3 + 1] & 0x0F) << 2) +
                               ((bytes[i*3 + 2] & 0xC0) >> 6)];
        b64[i*4 + 3] = b64_ref[  bytes[i*3 + 2] & 0x3F];
    }

    if (num_bytes % 3 == 1) {
        b64[i*4    ] = b64_ref[(bytes[i*3] & 0xFC) >> 2];
        b64[i*4 + 1] = b64_ref[(bytes[i*3] & 0x03) << 4];
        b64[i*4 + 2] = '=';
        b64[i*4 + 3] = '=';
    }
    
    else if (num_bytes % 3 == 2) {
        b64[i*4    ] = b64_ref[ (bytes[i*3    ] & 0xFC) >> 2];
        b64[i*4 + 1] = b64_ref[((bytes[i*3    ] & 0x03) << 4) +
                               ((bytes[i*3 + 1] & 0xF0) >> 4)];
        b64[i*4 + 2] = b64_ref[ (bytes[i*3 + 1] & 0x0F) << 2];
        b64[i*4 + 3] = '=';
    }
}


size_t num_b64_chars(const size_t num_bytes) {
    return ((8 * num_bytes) / 6) + (3 * (num_bytes % 3 != 0));
}


char* hex_to_base64(const char* const hex, char* const base_64) {
    const size_t num_bytes = strlen(hex) / 2;
    const size_t num_chars = num_b64_chars(num_bytes);

    base_64_encode(hex_str_to_bytes(hex), num_bytes, base_64);
    base_64[num_chars] = '\0';
    return base_64;
}
