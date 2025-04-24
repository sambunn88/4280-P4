//parser.h
//Sam Bunn 3/24/25
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "token.h"
#endif
#include "scanner.h"
#ifndef _NODE_H_
#define _NODE_H_
#include "node.h"
#endif
using namespace std;

void preprocess(string);
node_t* getNode(string);
node_t* S();
node_t* A();
node_t* B();
node_t* C();
node_t* D();
node_t* E();
node_t* F();
node_t* G();
node_t* parser(string);