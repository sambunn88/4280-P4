//testTree.h
//Sam Bunn 4/13/2025
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
using namespace std;

void traversePreOrder(node_t*, string);
void preOrder(node_t*, FILE*, int);