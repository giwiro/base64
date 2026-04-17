#ifndef BASE64_H
#define BASE64_H

size_t
base64_encode(const char *input, size_t input_size, char *output);

size_t
base64_decode(const char *input, size_t input_size, char *output);

#endif