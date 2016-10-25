
CC = g++
CFLAGS = -g -std=c++11

all: tester

IniReader.o: IniReader.h IniReader.cxx
	$(CC) $(CFLAGS) -c IniReader.cxx

tester.o: tester.cxx
	$(CC) $(CFLAGS) -c tester.cxx

tester: tester.o IniReader.o
	$(CC) $(CFLAGS) -o tester tester.o IniReader.o



