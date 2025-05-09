//parser.cpp
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
#include "parser.h"
using namespace std;

string tokenNames[] = {"End of File Token", "t1 token", "t2 token", "t3 token"}; 
FILE* processFileRead;
token* tk;

//Preprocess file with comments and ensure valid characters. 
void preprocess(string fileName) {
    string processedFileName = "processFile";
    FILE* processFile;
    const char* inputFileCStr = processedFileName.c_str();
    processFile = fopen(inputFileCStr,"w+");
    const char* inputFileCStr2 = fileName.c_str();
    FILE* inFile = fopen(inputFileCStr2, "r");

    //Preprocess file and create new file without comments ensure that only valid characters are in the file
    char ch;
    bool comment = false;
    while ((ch = fgetc(inFile)) != EOF) {
    if (comment) {
        //During comment
        if (ch == '*') {
            comment = false;
            fprintf(processFile, " ");
        }
        else if (ch == '\n') {
            fprintf(processFile, "\n");
        }
        else {
            fprintf(processFile, " ");
        }
    }
    else {
        if (ch == '*') {
            comment = true;
            fprintf(processFile, " ");
        }
        else if (isdigit(ch) || isalpha(ch) || isspace(ch) || (ch >= 33 && ch <= 43))  { //OR1
            fprintf(processFile, "%c", ch);
        }  
        else {
            fprintf(stderr, "\nProcessing Error: Invalid Character Found  %c\n", ch); 
            exit (1); 
        }
    }
    }

    fclose(processFile);
    fclose(inFile);
}

//This function generates a node with the label passed in it
node_t* genNode(string label) {
    node_t* node = new node_t;
    node->label = label;
    return node;
}

//This function is the process that runs to parse the file. 
node_t* parser(string fileName) {
    //Preprocess Input File
    preprocess(fileName);

    //Open preprocessed file for reading
    string processedFileName = "processFile";
    const char* inputFileCStr = processedFileName.c_str();
    processFileRead = fopen(inputFileCStr,"r");

    tk = scanner(processFileRead);
    node_t* root = S();
    if (tk->type != 0) {
        fprintf(stderr, "\nParser Error: EOF Token Expected  \n"); 
        exit (1); 
    }

    //Close processFile
    fclose(processFileRead);
    remove(inputFileCStr);

    return root;
}

node_t* S() {
    node_t* p = genNode("S"); 
    p->child1 = A();
    if (tk->type == 1 && tk->text[0] == '(') {
        p->child2 = genNode("t1");
        p->child2->tk = tk;
        tk = scanner(processFileRead);
    }
    else {
        fprintf(stderr, "Parser Error: Expected '(', found '%s'\n", tk->text.c_str());
        exit(1);
    }
    p->child3 = B();
    p->child4 = B();
    if (tk->type == 1 && tk->text[0] == ')') {
        p->child5 = genNode("t1");
        p->child5->tk = tk;
        tk = scanner(processFileRead);
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: Expected ')', found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}

node_t* A() {
    node_t* p = genNode("A"); 
    if (tk->type == 1 && tk->text[0] == '\"') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk=scanner(processFileRead);
        if (tk->type == 2) {
            p->child2 = genNode("t2");
            p->child2->tk = tk;
            tk=scanner(processFileRead);
            return p;
        }
        else {
            fprintf(stderr, "Parser Error: Expected t2 token, found '%s'\n", tk->text.c_str());
            exit(1);
        }
    }
    else {
        p->child1 = genNode("empty");
        return p;
    }
    return NULL;
}

node_t* B() {
    node_t* p = genNode("B"); 
    if (tk->text[0] == '\"') {
        p->child1 = A();
        return p;
    }
    else if (tk->text[0] == '#' || tk->text[0] == '!') {
        p->child1 = C();
        return p;
    }
    else if (tk->text[0] == '$') {
        p->child1 = D();
        return p;
    }
    else if (tk->text[0] == '\'') {
        p->child1 = E();
        return p;
    }
    else if (tk->type == 2) {
        p->child1 = G();
        return p;
    }
    else if (tk->type == 1 && tk->text[0] == '(') {
        node_t* s = S();
        node_t* p = genNode("B");
        p->child1 = s;
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: No valid entry for B nonterminal");
        exit(1);
    }
    return NULL;
}

node_t* C() {
    node_t* p = genNode("C"); 
    if (tk->type == 1 && tk->text[0] == '#') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        if (tk->type == 2) {
            p->child2 = genNode("t2");
            p->child2->tk = tk;
            tk = scanner(processFileRead);
            return p;
        }
        else {
            fprintf(stderr, "Parser Error: Expected t2 token, found '%s'\n", tk->text.c_str());
            exit(1);
        }
        return NULL;
    }
    else if (tk->type == 1 && tk->text[0] == '!') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        p->child2 = F();
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: Expected # or !, found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}

node_t* D() {
    node_t* p = genNode("D"); 
    if (tk->type == 1 && tk->text[0] == '$') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        p->child2 = F();
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: Expected $, found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}

node_t* E() {
    node_t* p = genNode("E"); 
    if (tk->type == 1 && tk->text[0] == '\'') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        p->child2 = F();
        p->child3 = F();
        p->child4 = F();
        p->child5 = B();
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: Expected \', found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}

node_t* F() {
    node_t* p = genNode("F"); 
    if (tk->type == 1 && tk->text[0] == '&') {
        p->child1 = genNode("t1");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        p->child2 = F();
        p->child3 = F();
        return p;
    }
    else if (tk->type == 2) {
        p->child1 = genNode("t2");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        return p;
    }
    else if (tk->type == 3) {
        p->child1 = genNode("t3");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        return p;
    }
    else {
        fprintf(stderr, "Parser Error: Expected t2 token, t3 token, or &, found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}

node_t* G() {
    node_t* p = genNode("G"); 
    if (tk->type == 2) {
        p->child1 = genNode("t2");
        p->child1->tk = tk;
        tk = scanner(processFileRead);
        if (tk->type == 1 && tk->text[0] == '%') {
            p->child2 = genNode("t1");
            p->child2->tk = tk;
            tk = scanner(processFileRead);
            p->child3 = F();
            return p;
        }
        else {
            fprintf(stderr, "Parser Error: Expected %, found '%s'\n", tk->text.c_str());
            exit(1);
        }
    }
    else {
        fprintf(stderr, "Parser Error: Expected t2 token, found '%s'\n", tk->text.c_str());
        exit(1);
    }
    return NULL;
}