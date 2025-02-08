
CC = g++
CFLAGS = -g -std=c++20

all: tester tester_utf tester_pool maze

tester: tester.cpp Makefile
	$(CC) $(CFLAGS) -o tester tester.cpp

tester_utf: tester_utf.cpp Makefile
	$(CC) $(CFLAGS) -o tester_utf tester_utf.cpp

tester_pool: tester_pool.cpp my_pool.h Makefile
	$(CC) $(CFLAGS) -o tester_pool tester_pool.cpp -pthread

maze: maze.cpp Makefile
	$(CC) $(CFLAGS) -o maze maze.cpp



mem_eater: mem_eater.cpp  Makefile
	$(CC) $(CFLAGS) -o mem_eater mem_eater.cpp -pthread


