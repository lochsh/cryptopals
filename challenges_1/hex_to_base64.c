#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK_4_LSB 0x0F
#define ASCII_NUM_LETTER_BOUND 64

#define ASCII_TO_B64_CAPS  65
#define ASCII_TO_B64_LOWER 71
#define ASCII_TO_B64_NUMS  -4
#define ASCII_TO_B64_62   -19
#define ASCII_TO_B64_63   -16


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


/* Deal with any leftover bytes after dividing bytes into six bit chunks.
 *
 * If the number of bytes in a byte array is not divisible by 3, then we need
 * to specially handle dividing it into 6 bit chunks.  Zero-padding is added
 * until there is a multiple of 3 bytes, which will in turn ensure full 6 bit
 * chunks can be created.
 */
static void process_leftover_bytes(const uint8_t* const bytes, uint8_t* chunks,
                                   const size_t num_bytes,
                                   const size_t num_chunks) {
    const size_t leftover_bytes = num_bytes % 3;

    if (leftover_bytes != 0) {
        size_t i = num_bytes - leftover_bytes;
        size_t j = num_chunks - 4;

        uint32_t x = (uint32_t) bytes[i] << 16;

        if (leftover_bytes == 2) {
            x += (uint32_t) bytes[i+1] << 8;
            chunks[j+2] = (x & 0x000FC0) >>  6;
        }

        chunks[j  ] = (x & 0xFC0000) >> 18;
        chunks[j+1] = (x & 0x03F000) >> 12;
    }
}


size_t num_b64_chars(const size_t num_bytes) {
    return ((8 * num_bytes) / 6) + (3 * (num_bytes % 3 != 0));
}


/* Divide bytes into 6-bit chunks, in preparation for base 64 encoding.
 *
 * In base 64 encoding, each character represents 6 bits.  We therefore must
 * extract 6-bit long chunks from the bytes we are encoding.
 */
static uint8_t* six_bit_chunks(uint8_t* const bytes, const uint8_t num_bytes) {
    const size_t num_chunks = num_b64_chars(num_bytes);
    uint8_t* chunks = (uint8_t*) malloc(num_chunks);

    if (chunks == NULL) {
        perror("Error allocating memory");
        return NULL;
    }

    size_t i, j;
    for (i = 0, j = 0; i < num_bytes; i += 3, j += 4) {

        uint32_t x = ((uint32_t) bytes[i  ] << 16) +
                     ((uint32_t) bytes[i+1] <<  8) +
                     ((uint32_t) bytes[i+2]);

        chunks[j  ] = (x & 0xFC0000) >> 18;
        chunks[j+1] = (x & 0x03F000) >> 12;
        chunks[j+2] = (x & 0x000FC0) >>  6;
        chunks[j+3] = (x & 0x00003F);
    }

    process_leftover_bytes(bytes, chunks, num_bytes, num_chunks);
    free(bytes);
    return chunks;
}


char* hex_to_base64(const char* const hex, char* const base_64) {
    const size_t num_bytes = strlen(hex) / 2;
    uint8_t* const chunks = six_bit_chunks(hex_str_to_bytes(hex),
                                           num_bytes);
    const size_t num_chars = num_b64_chars(num_bytes);

    for (size_t i = 0; i < num_chars; i++) {
        base_64[i] = chunks[i]
                 + ASCII_TO_B64_CAPS  * (chunks[i] <= 25)
                 + ASCII_TO_B64_LOWER * (26 <= chunks[i] && chunks[i] <= 51)
                 + ASCII_TO_B64_NUMS  * (52 <= chunks[i] && chunks[i] <= 61)
                 + ASCII_TO_B64_62    * (chunks[i] == 62)
                 + ASCII_TO_B64_63    * (chunks[i] == 63);
    }

    free(chunks);
    
    for (uint8_t i = 1; i <= num_bytes % 3; i++) {
        base_64[num_chars - i] = '=';
    }
    base_64[num_chars] = '\0';
    return base_64;
}
