CC = gcc
CFLAGS = -Wall -Wextra -fstack-protector-strong
TARGET = b64

all: prod

dev: CFLAGS += -g -O0
dev: main unit_test

prod: CFLAGS += -O3 -DNDEBUG
prod: main

b64.o : base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c -o base64.o

unit_test : test.c b64.o
	$(CC) $(CFLAGS) test.c base64.o -o test

main : main.c b64.o
	$(CC) $(CFLAGS) main.c base64.o -o $(TARGET)


.PHONY: clean test
clean :
	rm -rf *.out *.o $(TARGET) $(TARGET).dSYM/ test

test : unit_test
	./test