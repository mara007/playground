
CC = g++
CFLAGS = -g -std=c++11

all: tester

tester: tester.cpp
	$(CC) $(CFLAGS) -o tester tester.cpp


