
CC = g++
CFLAGS = -g

all: tester

tester: tester.cpp
	$(CC) $(CFLAGS) -o tester tester.cpp


