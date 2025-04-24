//testTree.cpp
//Sam Bunn 4/13/25
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
using namespace std;

//Runs the Post Order traversal recursively
void preOrder(node_t* root, FILE* file, int level) {
    //Traverse Array and Print to File
    if (root==NULL) return;
    fprintf(file,"%*c%s ",level*4,' ',root->label.c_str()); 
    if (root->tk != NULL && !root->tk->text.empty()) {
        fprintf(file,"%s ",root->tk->text.c_str()); 
    }
    fprintf(file,"\n");
    preOrder(root->child1,file,level+1);
    preOrder(root->child2,file,level+1);
    preOrder(root->child3,file,level+1);
    preOrder(root->child4,file,level+1);
    preOrder(root->child5,file,level+1);
}

//Method to call to run Pre Order Traversal. Sets up file and then calls helper method
void traversePreOrder(node_t* root, string base) {
    //Determine Filename
    string fileName;
    fileName.append(base);
    fileName.append(".preorder");
    
    //Create and Open File
    FILE* preOrderFile;
    const char* fileCStr = fileName.c_str();
    preOrderFile = fopen(fileCStr, "w");

    preOrder(root, preOrderFile,0);
        
    fclose(preOrderFile);
}

