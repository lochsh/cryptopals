#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK_4_LSB 0x0F
#define ASCII_NUM_LETTER_BOUND 64


uint32_t offset_mask(const uint8_t num_bits, const uint8_t start_bit) {
    return ((uint32_t) (1 << num_bits) - 1) << (32 - start_bit - num_bits);
}


uint8_t hex_char_to_padded_byte(const char hex_char) {
    return (hex_char & MASK_4_LSB) + (hex_char / ASCII_NUM_LETTER_BOUND) * 9;
}


uint8_t hex_char_pair_to_byte(const char hex_ms, const char hex_ls) {
    return (hex_char_to_padded_byte(hex_ms) << 4) +
            hex_char_to_padded_byte(hex_ls);
}


uint8_t* hex_str_to_bytes(const char* const hex_str) {
    const int num_bytes = strlen(hex_str) / 2;
    uint8_t *bytes = (uint8_t *) malloc(num_bytes);

    if (bytes == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    for (int i = 0; i < num_bytes; i++) {
        bytes[i] = hex_char_pair_to_byte(hex_str[i*2], hex_str[i*2 + 1]);
    }

    return bytes;
}


uint8_t* six_bit_chunks(const uint8_t* const bytes, const uint8_t num_bytes) {
    const int num_chunks = 8 * num_bytes / 6;
    uint8_t* chunks = (uint8_t*) malloc(num_chunks);

    if (chunks == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    for (int i = 0; i < num_chunks - 1; i += 3) {
        uint32_t x = ((uint32_t) bytes[i] << 24) +
                     ((uint32_t) bytes[i+1] << 16) +
                     ((uint32_t) bytes[i+2] << 8);

        for (int j = 0; j < i + 4; j++) {
            chunks[j] = (x & offset_mask(6, j*6)) >> (32 - (j + 1)*6);
        }
    }

    return chunks;
}


uint8_t* base64_to_ascii(const uint8_t* const base_64, const uint8_t n) {
    uint8_t* ascii = (uint8_t*) malloc(n);

    if (ascii == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        ascii[i] = base_64[i] + 65 * (base_64[i] <= 25)
                             + 71 * (26 <= base_64[i] && base_64[i] <= 51) 
                             - 4 * (52 <= base_64[i] && base_64[i] <= 61)
                             - 19 * (base_64[i] == 62)
                             - 16 * (base_64[i] == 63);
    }
    
    return ascii;
}



void print_base64(const uint8_t* const bytes, const uint8_t num_bytes) {
    const uint8_t* const chunks = six_bit_chunks(bytes, num_bytes);
    const int num_chunks = 8 * num_bytes / 6;

    for (int i = 0; i < num_chunks; i++) {
        printf("%c", chunks[i] + 65 * (chunks[i] <= 25)
                               + 71 * (26 <= chunks[i] && chunks[i] <= 51)
                               - 4 * (52 <= chunks[i] && chunks[i] <= 61)
                               - 19 * (chunks[i] == 62)
                               - 16 * (chunks[i] == 63));
    }
}


void print_hex_as_base64(const char* const hex_str) {
    const int num_bytes = strlen(hex_str) / 2;
    uint8_t* bytes = hex_str_to_bytes(hex_str);
    print_base64(bytes, num_bytes);
}
