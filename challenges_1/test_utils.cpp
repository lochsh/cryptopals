#include <stdint.h>
#include <string.h>
#include "utils.c"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE( "Hex char is converted to padded byte" ) {
    const char l_letters[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    const char u_letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    for (uint32_t i = 0; i < 10; i++) {
        REQUIRE( hex_char_to_padded_byte((char)i) == i );
    }

    for (uint32_t i = 0; i < 6; i++) {
        REQUIRE( hex_char_to_padded_byte(l_letters[i]) == i + 10 );
        REQUIRE( hex_char_to_padded_byte(u_letters[i]) == i + 10 );
    }
}


TEST_CASE( "Hex char pair is converted to byte" ) {
    REQUIRE( hex_char_pair_to_byte('d', 'e') == 222 );
}


TEST_CASE( "Hex string is converted to bytes" ) {
    const uint8_t expected[] = {0xde, 0xad, 0xbe, 0xef};
    REQUIRE( memcmp(hex_decode((char*)"deadbeef"), expected,
                    sizeof(expected)) == 0 );
}


TEST_CASE( "Getting number of b64 chars from hex string" ) {
    REQUIRE( num_b64_chars(1) == 4 );
    REQUIRE( num_b64_chars(2) == 4 );
    REQUIRE( num_b64_chars(3) == 4 );

    REQUIRE( num_b64_chars(4) == 8 );
    REQUIRE( num_b64_chars(5) == 8 );
    REQUIRE( num_b64_chars(6) == 8 );

    REQUIRE( num_b64_chars(10) == 16 );
    REQUIRE( num_b64_chars(48) == 64 );
}


TEST_CASE( "hex to base 64" ) {
    char base_64[] = "q83v";
    char hex_str[] = "ABCDEF";

    char* result = (char*) malloc(num_b64_chars(strlen(hex_str)/2) + 1);
    if (result == NULL) {
        perror("Error allocating memory");
    }

    hex_to_base64(hex_str, result);

    for (size_t i = 0; i < sizeof(base_64); i++) {
        REQUIRE( result[i] == base_64[i]);
    }
    
    free(result);
}


TEST_CASE( "hex to base 64 with 1 leftover byte" ) {
    const char base_64[] = "q83vEg==";
    const char hex_str[] = "ABCDEF12";

    char* result = (char*) malloc(num_b64_chars(strlen(hex_str)/2) + 1);
    if (result == NULL) {
        perror("Error allocating memory");
    }

    hex_to_base64(hex_str, result);

    for (size_t i = 0; i < sizeof(base_64); i++) {
        REQUIRE( result[i] == base_64[i]);
    }
    
    free(result);
}


TEST_CASE( "hex to base 64 with 2 leftover bytes" ) {
    const char base_64[] = "q83vEjQ=";
    const char hex_str[] = "ABCDEF1234";

    char* result = (char*) malloc(num_b64_chars(strlen(hex_str)/2) + 1);
    if (result == NULL) {
        perror("Error allocating memory");
    }

    hex_to_base64(hex_str, result);

    for (size_t i = 0; i < sizeof(base_64); i++) {
        REQUIRE( result[i] == base_64[i]);
    }
    
    free(result);
}


TEST_CASE( "hex to base 64 acceptance test" ) {
    const char hex[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65"
                       "206120706f69736f6e6f7573206d757368726f6f6d";
    const char b64[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG"
                       "11c2hyb29t";
    char* result = (char*) malloc(num_b64_chars(strlen(hex)/2) + 1);
    if (result == NULL) {
        perror("Error allocating memory");
    }

    hex_to_base64(hex, result);

    for (uint32_t i = 0; i < strlen(b64); i++) {
        REQUIRE( b64[i] ==  result[i] );
    }

    free(result);
}


TEST_CASE( "fixed xor acceptance test" ) {
    const char hex_x[] = "1c0111001f010100061a024b53535009181c";
    const char hex_y[] = "686974207468652062756c6c277320657965";

    char* xor_hex = (char*) malloc(strlen(hex_x)/2 + 1);

    if (xor_hex == NULL) {
        perror("Error allocating memory");
        return;
    }

    hex_fixed_xor(hex_x, hex_y, xor_hex);
    const char* const expected = "746865206b696420646f6e277420706c6179";

    for (size_t i = 0; i < strlen(hex_x); i++) {
        REQUIRE( xor_hex[i] == expected[i] );
    }
}
