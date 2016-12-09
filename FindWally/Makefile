CC=g++-5 -fopenmp 

CFLAGS=-c -std=c++11

all: wally

wally: main.o image.o matchimage.o largeimage.o
	$(CC) main.o image.o matchimage.o largeimage.o -o findwally.out

main.o: Main.cpp
	$(CC) $(CFLAGS) Main.cpp
image.o: Image.cpp
	$(CC) $(CFLAGS) Image.cpp
matchimage.o: MatchImage.cpp
	$(CC) $(CFLAGS) MatchImage.cpp
largeimage.o: LargeImage.cpp
	$(CC) $(CFLAGS) LargeImage.cpp
clean:
	rm *o findwally.out
