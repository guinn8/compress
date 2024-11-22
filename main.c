#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "compress.h"

#define DEFAULT_BUFFER_LEN 1024



int main(void) {
    size_t index = 0;
    size_t buf_len = DEFAULT_BUFFER_LEN;
    uint8_t *buffer = malloc(buf_len);
    if(buffer == NULL) {
        return EXIT_FAILURE;
    }

    while (1) {
        int value;
        int result = scanf("%d%*[,]", &value); // skip over any commas, spaces, or newlines
        if (result == EOF) {
            break;
        } else if (result != 1) {
            perror("Invalid input. Please enter numbers separated by commas.");
            free(buffer);
            return EXIT_FAILURE;
        }

        if (value < 0x00 || value > 0x7F) {
            perror("Invalid input. Please enter only values in range 0 - 127.");
            free(buffer);
            return EXIT_FAILURE;
        }

        if (index >= buf_len) {
            size_t buf_len = buf_len * 2; // double everytime we need more space
            uint8_t *new_buffer = realloc(buffer, buf_len);
            if (new_buffer == NULL) {
                perror("Error allocating memory");
                free(buffer);
                return EXIT_FAILURE;
            }
            buffer = new_buffer;
        }

        buffer[index++] = (uint8_t)value;
    }

    byte_compress(buffer, index);

    free(buffer);
    return EXIT_SUCCESS;
}
