#include <stdint.h>
#include "hex_to_base64.c"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE( "Hex char is converted to padded byte" ) {
    const char l_letters[] = {'a', 'b', 'c', 'd', 'e', 'f'};
    const char u_letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};

    for (int i = 0; i < 10; i++) {
        REQUIRE( hex_char_to_padded_byte((char)i) == i );
    }

    for (int i = 0; i < 6; i++) {
        REQUIRE( hex_char_to_padded_byte(l_letters[i]) == i + 10 );
        REQUIRE( hex_char_to_padded_byte(u_letters[i]) == i + 10 );
    }
}


TEST_CASE( "Hex char pair is converted to byte" ) {
    REQUIRE( hex_char_pair_to_byte('d', 'e') == 222 );
}


TEST_CASE( "Hex string is converted to bytes") {
    const uint8_t expected[] = {0xde, 0xad, 0xbe, 0xef};
    REQUIRE( memcmp(hex_str_to_bytes((char*)"deadbeef"), expected,
                    sizeof(expected)) == 0 );
}


TEST_CASE( "Offset mask returns appropriate masks" ) {
    REQUIRE( offset_mask(0, 0) == 0 );
    REQUIRE( offset_mask(6, 0) == 4227858432 );
    REQUIRE( offset_mask(1, 31) == 1 );
}


TEST_CASE( "Dividing all zero bytes into 6 bit chunks" ) {
    const int num_bytes = 3;
    const int num_chunks = 8 * num_bytes / 6;

    uint8_t bytes[num_bytes] = {0};
    const uint8_t chunks[num_chunks] = {0};

    uint8_t* result = six_bit_chunks(bytes, num_bytes);
    for (int i = 0; i < num_chunks; i++) {
        REQUIRE( result[i] == chunks[i] );
    }
}


TEST_CASE( "Dividing bytes into 6 bit chunks" ) {
    const int num_bytes = 3;
    const int num_chunks = 8 * num_bytes / 6;

    uint8_t bytes[] = {0, 1, 2};
    const uint8_t chunks[] = {0, 0, 4, 2};

    uint8_t* result = six_bit_chunks(bytes, num_bytes);
    for (int i = 0; i < num_chunks; i++) {
        REQUIRE( result[i] == chunks[i] );
    }
}
