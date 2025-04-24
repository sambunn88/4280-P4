//scanner.cpp
//Sam bunn 3/24/25
#include <iostream>
#include <ctype.h>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <fstream>
#ifndef _TOKEN_H_ //start OR1
#define _TOKEN_H_
#include "token.h"
#endif  //endOR1
using namespace std;

//FSA Table 
int table[6][6] = {{1000, 1, -1, 3, 5, 0},{-2, -2, 2, -2, -2, -2},{1003, 1003, 2, 1003, 1003, 1003},{-3, -3, 4, -3, -3, -3},{1002, 1002, 4, 1002, 1002, 1002},{1001, 1001, 1001, 1001, 1001, 1001}};

//Keep track of the line between calls
static int line = 1;

int resolveChar(char ch) {
    if (ch == EOF) {
        return 0;
    }
    else if (isalpha(ch)) {
        return 1;
    }
    else if (isdigit(ch)) {
        return 2;
    }
    else if (ch == '+') {
        return 3;
    }
    else if (ch >= 33 && ch <= 41) {
        return 4;
    }
    else if (isspace(ch)) {
        return 5;
    }
    else {
        return -1;
    }
}

//FSA Table number, character input, line number
token* intToToken(int typeNum, string str, int lineNum) {
    token* tk = new token;
    tk->type = typeNum-1000;
    tk->text = str;
    tk->line = lineNum;
    return tk;
}

token* scanner(FILE* inFile) {
    int state = 0;
    int nextState;
    string str = ""; //OR1
    static char nextChar = fgetc(inFile); //OR1

    while (!(state > 1000) || (state < 0)) {
        nextState = table[state][resolveChar(nextChar)];

        //Error State, print error message and exit
        if (nextState < 0) {
            //Switch statement for errors
            switch (nextState) {
                case -1:
                    cout << "SCANNER ERROR: Number not expected at beginning of token" << endl;
                    exit(1);
                    break;
                case -2:
                    cout << "SCANNER ERROR: Digit Expected after Alpha Char" << endl;
                    exit(1);
                    break;
                case -3:
                    cout << "SCANNER ERROR: Digit Expected after +" << endl;
                    exit(1);
                    break;
                default:
                    cout << "SCANNER ERROR: Unknown Error Encountered" << endl;
                    exit(1);
                    break;
            }
        }

        //If state is token
        else if (nextState >= 1000) {
            // make and return token
            return intToToken(nextState, str, line);
        }

        //Not Final
        else {
            state = nextState;
            if (!isspace(nextChar))
                str.append(1, nextChar);
            if (nextChar == '\n') { //Start OR1
                line++;
            }   //End OR1
            nextChar = fgetc(inFile); //OR1
            
        }
    }
    return nullptr; //OR1
}


