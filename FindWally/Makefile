CC=g++-5 -fopenmp

CFLAGS=-c -std=c++11

all: wally

wally: main.o image.o
	$(CC) main.o image.o -o findwally.out

main.o: Main.cpp
	$(CC) $(CFLAGS) Main.cpp
image.o: Image.cpp
	$(CC) $(CFLAGS) Image.cpp
clean:
	rm *o findwally.out
