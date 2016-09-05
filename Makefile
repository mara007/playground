
CC = g++
CFLAGS = -g -std=c++11

all: tester intervals

tester: tester.cpp
	$(CC) $(CFLAGS) -o tester tester.cpp

intervals: intervals.cpp
	$(CC) $(CFLAGS) -o intervals intervals.cpp


