#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "base64.h"

#define READ_BUFFER_SIZE 5

int
parse_args(int argc, char *argv[], bool *is_decoding) {
    if (argc > 2) {
        printf("[Error] Wrong argument\n");
        return 1;
    }

    char *opt = argc == 2 ? argv[1] : "";

    if (argc == 2 && strcmp(opt, "-d") != 0) {
        printf("[Error] Wrong argument\n");
        printf("Usage:  b64 [-d] <input>\n");
        printf("  -d        decode input\n");
        return 1;
    }

    if (strcmp(opt, "-d") == 0) {
        *is_decoding = true;
    } else {
        *is_decoding = false;
    }

    return 0;
}

void
free_all(char *input, char *output, char *input_buffer) {
    free(input);
    free(output);
    free(input_buffer);
}

int
read_input(char *input, size_t *input_size, char *input_buffer) {
    while(fgets(input_buffer, READ_BUFFER_SIZE * sizeof(char), stdin)) {
        const size_t len = strlen(input_buffer);

        char *resized_input = realloc(input, *input_size + len);
        if (resized_input == NULL) {
            return 1;
        }

        input = resized_input;
        memcpy(input + *input_size, input_buffer, len);
        *input_size += len;
    }

    return 0;
}

int
main(int argc, char *argv[]) {
    bool is_decoding = false;

    if (parse_args(argc, argv, &is_decoding) != 0) {
        return 1;
    }

    size_t input_size = 0;
    char *input = malloc(0);
    char *output = malloc(0);
    char *input_buffer = malloc(READ_BUFFER_SIZE * sizeof(char));

    if (
        read_input(input, &input_size, input_buffer) != 0 ||
        input_size == 0
    ) {
        printf("[Error] Could not read input\n");
        free_all(input, output, input_buffer);
        return 1;
    }

    size_t output_size = 0;

    if (is_decoding) {
        output_size = base64_decode(input, input_size, output);
    } else {
        output_size = base64_encode(input, input_size, output);
    }

    if (output_size == 0) {
        free_all(input, output, input_buffer);
        return 1;
    }

    printf("%s\n", output);

    free_all(input, output, input_buffer);
    return 0;
}