
CC = g++
CFLAGS = -g -std=c++11

all: threads

clean:
	rm -Rf *o *exe

threads: threads.cpp
	$(CC) $(CFLAGS) -o threads threads.cpp


