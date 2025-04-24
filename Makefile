CC = g++
CFLAGS = -g -Wall 
TARGET = P3

all: p3

p3: staticSemantics.o testTree.o scanner.o parser.o main.o
	$(CC) $(CFLAGS) staticSemantics.o testTree.o scanner.o parser.o main.o -o P3

main.o: main.cpp 
	$(CC) $(CFLAGS) -c main.cpp -o main.o

scanner.o: scanner.cpp scanner.h 
	$(CC) $(CFLAGS) -c scanner.cpp

parser.o: parser.cpp parser.h 
	$(CC) $(CFLAGS) -c parser.cpp

testTree.o: testTree.cpp testTree.h
	$(CC) $(CFLAGS) -c testTree.cpp

staticSemantics.o: staticSemantics.cpp staticSemantics.h
	$(CC) $(CFLAGS) -c staticSemantics.cpp

clean:
	rm -rf *.o *.preorder P3  

