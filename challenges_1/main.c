#include "hex_to_base64.c"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("\x1B[31m Please provide one hex encoded string\n");
    }

    else {
        hex_str_to_bytes(*argv);
    }

    return 0;
}
