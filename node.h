//node.h
//Sam Bunn 3/25/2025
#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "token.h"
#endif
#include "scanner.h"
using namespace std;

//Defines structure for node on tree
struct node_t {
    std::string label;
    token* tk;
    node_t* child1;
    node_t* child2;
    node_t* child3;
    node_t* child4;
    node_t* child5;
};