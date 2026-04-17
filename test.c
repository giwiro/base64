#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base64.h"

static void
test(const char *input, size_t input_size, const char *encoded, size_t encoded_size)
{
    char *output = malloc(0);
    base64_encode(input, input_size, output);

    // encode test
    assert(encoded_size == strlen(encoded));
    assert(strcmp(encoded, output) == 0);

    printf("  ✅ encode [%s 🟰 %s]\n", encoded, output);

    char *decoded = malloc(0);
    base64_decode(output, strlen(output), decoded);

    // decode test
    assert(input_size == strlen(decoded));
    assert(strcmp(decoded, input) == 0);

    printf("  ✅ decode [%s 🟰 %s]\n", decoded, input);

    free(output);
    free(decoded);
}


int main() {
    printf("base64 test:\n");

	test((char *)"", 0, "", 0);
	test((char *)"f", 1, "Zg==", 4);
	test((char *)"fo", 2, "Zm8=", 4);
	test((char *)"foo", 3, "Zm9v", 4);
	test((char *)"foob", 4, "Zm9vYg==", 8);
	test((char *)"fooba", 5, "Zm9vYmE=", 8);
	test((char *)"foobar", 6, "Zm9vYmFy", 8);
	test((char *)"Man", 3, "TWFu", 4);
    test((char *)"Ma", 2, "TWE=", 4);
    test((char *)"M", 1, "TQ==", 4);

    return 0;
}