
CC = g++
CFLAGS = -g -std=c++20

all: tester tester_utf tester_pool

tester: tester.cpp Makefile
	$(CC) $(CFLAGS) -o tester tester.cpp

tester_utf: tester_utf.cpp Makefile
	$(CC) $(CFLAGS) -o tester_utf tester_utf.cpp

tester_pool: tester_pool.cpp my_pool.h Makefile
	$(CC) $(CFLAGS) -o tester_pool tester_pool.cpp -pthread


mem_eater: mem_eater.cpp  Makefile
	$(CC) $(CFLAGS) -o mem_eater mem_eater.cpp -pthread


