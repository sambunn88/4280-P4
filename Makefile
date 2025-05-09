CC = g++
CFLAGS = -g -Wall 
TARGET = P4

all: p4

p4: codeGen.o staticSemantics.o testTree.o scanner.o parser.o main.o
	$(CC) $(CFLAGS) codeGen.o staticSemantics.o testTree.o scanner.o parser.o main.o -o P4

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

codeGen.o: codeGen.cpp codeGen.h
	$(CC) $(CFLAGS) -c codeGen.cpp

clean:
	rm -rf *.o *.preorder *.asm P4  

