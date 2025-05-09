//codeGen.cpp
//Sam Bunn 5/8/25

#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "token.h"
#endif
#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#endif
#include <iostream>
#include <stdio.h>
#include <string>
#include <queue>
#include <set>
using namespace std;

FILE* outputFile;
string activeVar;
static int tempCount = 0;
static int labelCount = 0;
static set<string> table;

inline void fatal(const char* string) {fprintf(stderr,"\nFatal: %s\n\n",string); exit(1); }
string newTemp();
string newLabel();
string t2tk(string);
int t3tk(string );
void traverse(node_t*);
void genEOF();
void codeGen(node_t*, string, set<string>);

