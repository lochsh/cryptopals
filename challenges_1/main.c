#include "hex_to_base64.c"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("\x1B[31m Please provide one hex encoded string\n");
    } else {
        uint8_t* base_64 = hex_to_base64(*argv);
    }

    return 0;
}
