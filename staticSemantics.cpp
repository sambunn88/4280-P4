//staticSemantics.cpp
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

static set<string> table; //OR2

//This funciton verifies that the provided string is not already in the table. Returns true if it is in the table, otherwise returns false. OR1
bool verify(string str) {
    return table.find(str) != table.end(); //OR1
}

void insert(token* tk) {
    //Verify that identifier isn't already declared
    if (verify(tk->text)) {
        fprintf(stderr, "\nSemantics Error: %s already declared. Line %i\n", tk->text.c_str(), tk->line); 
        exit (1); 
    }

    //Add to set
    table.insert(tk->text);
}

//Reused and modified from testTree.cpp. Checks for t2 tokens and adds to table as needed. 
void searchNodes(node_t* root) {
    //Traverse Array and Print to File
    if (root==NULL) return;
    //Check node
    //Check for A and C labels, because they have the //OR1
    if (root->label == "A" &&  root->child1->tk && root->child1->tk->text == "\"") {
        insert(root->child2->tk);
        return;
    }
    if (root->label == "C" &&  root->child1->tk && root->child1->tk->text == "#") {
        insert(root->child2->tk);
        return;
    }
    //Any other t2 tokens
    if (root->tk && root->tk->type == 2) {
        if (!verify(root->tk->text)) {
            fprintf(stderr, "\nSemantics Error: %s is not declared declared. Line %i\n", root->tk->text.c_str(), root->tk->line); 
            exit (1); 
        }
    }

    searchNodes(root->child1);
    searchNodes(root->child2);
    searchNodes(root->child3);
    searchNodes(root->child4);
    searchNodes(root->child5);
}

void printTable() {
    cout << "Symbol Table:" << endl;
    for (auto s : table) {
        cout << s << endl;
    }
}

set<string> staticSemantics(node_t* root) { 
    //Traverse tree and make table
    searchNodes(root);
    return table;

}
