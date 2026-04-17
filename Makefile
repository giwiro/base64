CC = gcc
CFLAGS = -O0 -g -Wall
TARGET = base64

all: main

base64.o : base64.c base64.h
	$(CC) $(CFLAGS) -c base64.c -o base64.o

main : main.c base64.o
	$(CC) $(CFLAGS) main.c base64.o -o $(TARGET)


.PHONY: clean
clean :
	rm -rf *.out *.o $(TARGET) $(TARGET).dSYM/