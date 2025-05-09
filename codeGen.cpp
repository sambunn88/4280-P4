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

FILE* asmOutputFile;
string activeVarTwo;
static int tempCount = 0;
static int labelCount = 0;
static set<string> table;

inline void fatal(const char* string) {fprintf(stderr,"\nFatal: %s\n\n",string); exit(1); }

string newTemp() {
    return "T" + to_string(tempCount++);
}

string newLabel() {
    return "L" + to_string(labelCount++);
}

//Changes t2 token to variable for ASM
string t2tk(string t2) {
    if(t2[0] != '+') {
        fatal("Expected a plus sign for the first character of a t2 token.");
    }
    t2[0] = 'p';
    return t2;
}

//Changes t3 token to int for ASM
int t3tk(string t3) {
    int intVal; // will hold the integer value of the t3 token
    int caseVal; // set to +1 if upper-case (positive value) and set to -1 if lower-case (negative value)
    
    // checking whole range from A to Z (ascii 65 - 122)
    if((t3[0] < 65) || (t3[0] > 122)) {
        fatal("Expected a letter for the first character of a t3 token.");
    }
    // checking for ascii non-alphabetical characters between Z and a (ascii 91 - 96)
    if((t3[0] > 90) && (t3[0] < 97)) {
        fatal("Expected a letter for the first character of a t3 token.");
    } 
    // upper-case letter, so integer will be positive
    if((t3[0] > 64) && (t3[0] < 91)) {
        caseVal = 1;
    }
    // lower-case letter, so integer will be negative      
    if((t3[0] > 96) && (t3[0] < 123)) {
        caseVal = -1;
    }
   
    t3[0] = '0'; // set to zero so that it will go away when convert string to integer
    
    intVal = atoi(t3.c_str()); // convert to integer
    intVal = intVal * caseVal; // set to negative if needed   
        
    return intVal;
}

void traverse(node_t* root) {
    //Return if empty node
    if (root==NULL) {
        return;
    }

    if (root) {
        fprintf(stderr, "[TRAVERSE] Node: %s\n", root->label.c_str());
    }

    //Check for lettered node
    if (root->label == "S") {
        traverse(root->child1); //A
        traverse(root->child3); //B
        traverse(root->child4); //B
        return;
    }
    else if (root->label == "A") {
        //A -> "t2 
        if (root->child1 && root->child1->label == "t1") {
            activeVarTwo = t2tk(root->child2->tk->text);
            fprintf(asmOutputFile, "LOAD 0\n");
            fprintf(asmOutputFile, "STORE %s\n", activeVarTwo.c_str());
            return;
        }
    }
    else if (root->label == "B") {
        //B -> S | C | D | E | G
        if (root->child1) traverse(root->child1);
        if (root->child2) traverse(root->child2);
        if (root->child3) traverse(root->child3);
        if (root->child4) traverse(root->child4);
        if (root->child5) traverse(root->child5);
        return;
    }
    else if (root->label == "C") {
        if (root->child1->tk->text == "#") {
            activeVarTwo = t2tk(root->child2->tk->text);
            fprintf(asmOutputFile,"READ %s\n", activeVarTwo.c_str());
            return;
        }
        else if (root->child1->tk->text == "!") {
            traverse(root->child2);
            fprintf(asmOutputFile, "MULT -1\n");
            fprintf(asmOutputFile, "STORE %s\n", activeVarTwo.c_str());
            return;
        }

    }
    else if (root->label == "D") {
        traverse(root->child2);
        fprintf(asmOutputFile, "WRITE %s\n", activeVarTwo.c_str());
        return;
    }
    else if (root->label == "E") {
        traverse(root->child2);
        string t1 = newTemp();
        fprintf(asmOutputFile, "STORE %s\n", t1.c_str());

        traverse(root->child3);
        string t2 = newTemp();
        fprintf(asmOutputFile, "STORE %s\n", t2.c_str());

        traverse(root->child4);
        string loopCounter = newTemp();
        fprintf(asmOutputFile,"STORE %s\n", loopCounter.c_str());

        string skipLabel = newLabel();
        string loopLabel = newLabel();

        // Check if F1 > F2
        fprintf(asmOutputFile, "LOAD %s\n", t1.c_str());
        fprintf(asmOutputFile, "SUB %s\n", t2.c_str());
        fprintf(asmOutputFile, "BRZPOS %s\n", skipLabel.c_str());  // if F1 - F2 <= 0, 
        
        // Check if loopCounter >= 1
        fprintf(asmOutputFile, "LOAD %s\n", loopCounter.c_str());
        fprintf(asmOutputFile, "SUB 1\n");
        fprintf(asmOutputFile, "BRNEG %s\n", skipLabel.c_str());

        fprintf(asmOutputFile, "%s:NOOP\n", loopLabel.c_str());

        // Decrement loop counter
        fprintf(asmOutputFile, "LOAD %s\n", loopCounter.c_str());
        fprintf(asmOutputFile, "SUB 1\n");
        fprintf(asmOutputFile, "STORE %s\n", loopCounter.c_str());

        // Continue loop if counter >= 1
        fprintf(asmOutputFile, "LOAD %s\n", loopCounter.c_str());
        fprintf(asmOutputFile, "SUB 1\n");
        fprintf(asmOutputFile, "BRPOS %s\n", loopLabel.c_str());

        fprintf(asmOutputFile, "%s: NOOP\n", skipLabel.c_str());

        return;
    }
    else if (root->label == "F") {
        if (root->child1->label == "t1" && root->child1->tk->text == "&") {
            traverse(root->child2);
            string temp = newTemp();
            fprintf(asmOutputFile, "STORE %s\n", temp.c_str());
            traverse(root->child3);
            fprintf(asmOutputFile, "ADD %s\n", temp.c_str());
            activeVarTwo = newTemp();
            fprintf(asmOutputFile, "STORE %s\n", temp.c_str());
            return;
        }
        else if (root->child1->label == "t2") {
            string var = t2tk(root->child1->tk->text);
            activeVarTwo = var;
            fprintf(asmOutputFile, "LOAD %s\n", var.c_str());  
            return;
        }
        else if (root->child1->label == "t3") {
            string temp = newTemp();
            fprintf(asmOutputFile, "LOAD %i\n", t3tk(root->child1->tk->text));
            fprintf(asmOutputFile, "STORE %s\n", temp.c_str());
            activeVarTwo = temp;
            return;
        }
    }
    else if (root->label == "G") {
        string var = t2tk(root->child1->tk->text);
        traverse(root->child3);
        fprintf(asmOutputFile, "STORE %s\n", var.c_str());
        return;
    }

    //Otherwise, check each child
    traverse(root->child1);
    traverse(root->child2);
    traverse(root->child3);
    traverse(root->child4);
    traverse(root->child5);
}

void genEOF() {
    fprintf(asmOutputFile, "STOP\n");
    
    //Print Named Variables
    for (auto s : table) {
        fprintf(asmOutputFile,"%s 0\n", t2tk(s).c_str());
    }

    //Print Temp Variables
    for (int i = 0; i < tempCount; i++) {
        fprintf(asmOutputFile, "T%i 0\n", i);
    }
}

//This function is called in main and runs the code generation
void codeGen(node_t* root, string fileName, set<string> semantics) {
    //Save semeantics set
    table = semantics;

    //Open .asm file for writing
    fileName.append(".asm");
    const char* fileCStr = fileName.c_str();
    asmOutputFile = fopen(fileCStr, "w");

    //Parse tree and create code
    traverse(root);

    //Print STOP and Variables
    genEOF();

    //Close File
    fclose(asmOutputFile);
}

