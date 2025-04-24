//scanner.h
//Sam Bunn 3/24/25
#include <iostream>
#include <ctype.h>
#include <cstdio>
#include <string>
#include <fstream>
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include "token.h"
#endif
using namespace std;

int resolveChar(char);
token* intToToken(int, string, int); //FA Table number, character input, line number
token* scanner(FILE*);

