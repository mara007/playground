
CC = g++
CFLAGS = -g -std=c++20

all: tester tester_utf tester_pool

tester: tester.cpp
	$(CC) $(CFLAGS) -o tester tester.cpp

tester_utf: tester_utf.cpp
	$(CC) $(CFLAGS) -o tester_utf tester_utf.cpp

tester_pool: tester_pool.cpp arbitrary_pool.h
	$(CC) $(CFLAGS) -o tester_pool tester_pool.cpp



