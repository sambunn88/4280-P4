//staticSemantics.h
//Sam Bunn 4/19/25
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

bool verify(string);
void insert(token);
void searchNodes(node_t*);
void printTable();
void staticSemantics(node_t*);

