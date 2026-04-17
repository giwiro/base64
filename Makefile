CC = gcc
CFLAGS = -Wall -Wextra
TARGET = b64

all: main unit_test

dev: CFLAGS += -g -O0
dev: all

prod: CFLAGS += -O3 -DNDEBUG
prod: all

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