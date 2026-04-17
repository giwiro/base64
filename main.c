#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "base64.h"

#define READ_BUFFER_SIZE 10

int main() {
    size_t input_size = 0;
    char *input = malloc(0);
    char *output = malloc(0);
    char *input_buffer = malloc(READ_BUFFER_SIZE * sizeof(char));

    while(fgets(input_buffer, READ_BUFFER_SIZE * sizeof(char), stdin)) {
        const size_t len = strlen(input_buffer);

        char *resized_input = realloc(input, input_size + len);
        if (resized_input == NULL) {
            printf("[Error] Could not resize input buffer\n");
            free(input_buffer);
            free(input);
            free(output);
            return 1;
        }

        input = resized_input;
        memcpy(input + input_size, input_buffer, len);
        input_size += len;
    }

    printf("!\n");

    base64_encode(input, input_size, output);

    free(input_buffer);
    free(input);
    free(output);

    return 0;
}