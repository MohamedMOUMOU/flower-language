#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

#include <fstream>

#include <iomanip>

#include "types.hpp"

using namespace std;

// Output functions
string spaces();
void generationOutput();
void printSymbolTable();
void stdOAndFileOutput(node * , node * );

extern vector < string > DATASECTION;
extern vector < string > CODESECTION;
extern vector < vector < string >> symbolTable;

#endif