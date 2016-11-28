#include "utils.c"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("\x1B[31m Please provide one hex encoded string\n");
    }
    
    else {
        char* const hex_str = argv[1];

        char* base_64 = (char*) malloc(num_b64_chars(strlen(hex_str)/2) + 1);
        if (base_64 == NULL) {
            perror("Error allocating memory");
            return -1;
        }

        printf("%s\n", hex_to_base64(hex_str, base_64));
        free(base_64);
    }

    return 0;
}
