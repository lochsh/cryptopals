#include <stdint.h>
#include "hex_to_base64.c"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE( "Hex char is converted to padded byte", "[hex]" ) {
    REQUIRE( hex_char_to_padded_byte('d') == 13 );
    REQUIRE( hex_char_to_padded_byte('e') == 14 );
    REQUIRE( hex_char_to_padded_byte('0') == 0 );
    REQUIRE( hex_char_to_padded_byte('8') == 8 );
}


TEST_CASE( "Hex char pair is converted to byte", "[hex]" ) {
    REQUIRE( hex_char_pair_to_byte('d', 'e') == 222);
}


TEST_CASE( "Hex string is converted to bytes", "[hex]" ) {
    uint8_t expected[] = {0xde, 0xad, 0xbe, 0xef};
    REQUIRE( memcmp(hex_str_to_bytes((char*)"deadbeef"), expected,
                    sizeof(expected)) == 0 );
}
