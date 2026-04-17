#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

#define BASE64_PAD '='
#define BASE64DE_FIRST '+'
#define BASE64DE_LAST 'z'

static const char base64en[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/',
};

static const char base64de[] = {
	/* nul, soh, stx, etx, eot, enq, ack, bel, */
	   255, 255, 255, 255, 255, 255, 255, 255,
	/*  bs,  ht,  nl,  vt,  np,  cr,  so,  si, */
	   255, 255, 255, 255, 255, 255, 255, 255,
	/* dle, dc1, dc2, dc3, dc4, nak, syn, etb, */
	   255, 255, 255, 255, 255, 255, 255, 255,
	/* can,  em, sub, esc,  fs,  gs,  rs,  us, */
	   255, 255, 255, 255, 255, 255, 255, 255,
	/*  sp, '!', '"', '#', '$', '%', '&', ''', */
	   255, 255, 255, 255, 255, 255, 255, 255,
	/* '(', ')', '*', '+', ',', '-', '.', '/', */
	   255, 255, 255,  62, 255, 255, 255,  63,
	/* '0', '1', '2', '3', '4', '5', '6', '7', */
	    52,  53,  54,  55,  56,  57,  58,  59,
	/* '8', '9', ':', ';', '<', '=', '>', '?', */
	    60,  61, 255, 255, 255, 255, 255, 255,
	/* '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', */
	   255,   0,   1,  2,   3,   4,   5,    6,
	/* 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', */
	     7,   8,   9,  10,  11,  12,  13,  14,
	/* 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', */
	    15,  16,  17,  18,  19,  20,  21,  22,
	/* 'X', 'Y', 'Z', '[', '\', ']', '^', '_', */
	    23,  24,  25, 255, 255, 255, 255, 255,
	/* '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', */
	   255,  26,  27,  28,  29,  30,  31,  32,
	/* 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', */
	    33,  34,  35,  36,  37,  38,  39,  40,
	/* 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', */
	    41,  42,  43,  44,  45,  46,  47,  48,
	/* 'x', 'y', 'z', '{', '|', '}', '~', del, */
	    49,  50,  51, 255, 255, 255, 255, 255
};

size_t
base64_encode(const char *input, size_t input_size, char *output) {
    size_t output_size = (size_t)ceil((double)input_size / 3) * 4;
    char *resized_output = realloc(output, output_size * sizeof(char) + 1);
    if (resized_output == NULL) {
        printf("[Error] Could not resize output buffer\n");
        return 0;
    }
    output = resized_output;

    unsigned int j = 0;

    for (int i = 0; i < (int)input_size; i+=3) {
        char f = input[i];
        char s = i + 1 < (int)input_size ? input[i+1] : 0;
        char t = i + 2 < (int)input_size ? input[i+2] : 0;

        // First digit
        output[j++] = base64en[(f >> 2) & 0x3F];

        // Second digit
        output[j++] = base64en[((f & 0x3) << 4) | ((s >> 4) & 0xF)];

        // Third digit
        output[j++] = s == 0 && t == 0 ?
            BASE64_PAD : base64en[((s & 0xF) << 2) | ((t >> 6) & 0x3)];

        // Fourth digit
        output[j++] = t == 0 ? BASE64_PAD : base64en[t & 0x3F];
    }

    output[j] = '\0';

    return j;
}

size_t
base64_decode(const char *input, size_t input_size, char *output) {
    if (input_size % 4 != 0) {
        printf("[Error] Bad input size for decoding\n");
        return 0;
    }

    size_t output_size = (size_t)((input_size / 4) * 3);
    char *resized_output = realloc(output, output_size * sizeof(char) + 1);
    if (resized_output == NULL) {
        printf("[Error] Could not resize output buffer\n");
        return 0;
    }
    output = resized_output;

    int j = 0;

    for (int i = 0; i < (int)input_size; i+=4) {
        if (
            input[i] < BASE64DE_FIRST || input[i] > BASE64DE_LAST ||
            input[i+1] < BASE64DE_FIRST || input[i+1] > BASE64DE_LAST ||
            input[i+2] < BASE64DE_FIRST || input[i+2] > BASE64DE_LAST ||
            input[i+3] < BASE64DE_FIRST || input[i+3] > BASE64DE_LAST
        ) {
            printf("[Error] Bad input for decoding\n");
            return 0;
        }

        char fi = base64de[(int)input[i]];
        char se = base64de[(int)input[i+1]];
        char th = base64de[(int)input[i+2]];
        char fo = base64de[(int)input[i+3]];

        // First digit
        output[j++] = ((fi & 0x3F) << 2) | ((se >> 4) & 0x3);

        // Second digit
        if (input[i+2] == BASE64_PAD && ((se & 0xF) == 0)) {
            continue;
        }
        output[j++] = ((se & 0xF) << 4) | ((th >> 2) & 0xF);

        // Third digit
        if (
            (input[i+2] == BASE64_PAD && input[i+3] == BASE64_PAD) ||
            ((input[i+3] == BASE64_PAD) && (th & 0x3) == 0)
        ) {
            continue;
        }
        output[j++] = ((th & 0x3) << 6) | (fo & 0x3F);
    }

    output[j] = '\0';

    return j;
}

