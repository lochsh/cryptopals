#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t* hex_str_to_bytes(char* hex_str) {
    int num_bytes = strlen(hex_str) / 2;

    uint8_t *bytes = (uint8_t *) malloc(num_bytes);
	if (bytes == NULL) {
		perror("Error allocating memory");
		return NULL;
	}

    size_t i;
    for (i = 0; i < num_bytes; i++) {
        sscanf(&hex_str[2*i], "%02hhx", &bytes[i]);
    }

    return bytes;
}
