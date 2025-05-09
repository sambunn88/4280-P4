//main.cpp
//Sam Bunn 4/19/25
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "token.h"
#endif
#include "scanner.h"
#include "parser.h"
#include "testTree.h"
#include "staticSemantics.h"
#include "codeGen.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
using namespace std;

int main (int argc, char **argv) {
    string fileName = "filename";
    FILE* temp;
    //Process Command Line Arguments
    if (argc==1) {
        //No File Name Specified
        const char* inputFileCStr = fileName.c_str();
        temp = fopen(inputFileCStr,"w+"); //OR2
        if (temp == NULL) {
            fprintf(stderr, "\nFatal: Input File Did Not Open"); 
            exit (2); 
        }

        //Read in file while checking for EOF
        char ch;
        while ((ch = getchar()) != EOF) {
            fprintf(temp, "%c", ch);
        }

        fclose(temp);
    }
    else if (argc==2) {
        //File Specified
        //Accept File and check that it opens
        fileName=argv[1];
        const char* inputFileCStr = fileName.c_str();
        temp = fopen(inputFileCStr, "r");
        if (temp == NULL) { 
            fprintf(stderr, "\nFatal: Input File Did Not Open"); 
            exit (2); 
        }
        fclose(temp);
    }
    else {
        fprintf(stderr, "\nFatal: Invalid Arguments Given"); 
        exit (3); 
    }
    

    //Run parser here
    node_t* tree = parser(fileName);

    //Tree output
    traversePreOrder(tree, fileName);

    //Run function to create symbol table
    set<string> symbolTable = staticSemantics(tree);

    //Generate ASM Code
    codeGen(tree, fileName, symbolTable);   


    //remove temporary file if it was used
    if (fileName == "filename") {
        remove("filename"); 
    }

    return 0;
}
