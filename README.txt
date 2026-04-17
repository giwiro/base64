base64 (small C project)
========================

Encode or decode stdin as Base64. The CLI binary is named "b64".

Requirements
------------
  * A C compiler (Makefile uses gcc)
  * POSIX-ish environment (stdio, malloc)

Build
-----
  make         Default target is "prod" (see below)
  make dev     Debug-friendly flags (-g -O0), builds "b64" and "test"
  make prod    Optimized release (-O3 -DNDEBUG), builds "b64" only
  make clean   Remove "b64", "test", objects, and local debug bundles

Run tests
---------
  make test    Builds the unit test binary (if needed) and runs ./test

Usage
-----
  Read from standard input; result is written to standard output with a
  trailing newline.

    ./b64 < file.bin           # encode
    ./b64 -d < encoded.txt     # decode
