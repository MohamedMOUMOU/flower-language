#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <math.h>
#include <string>
#include <unordered_map>
#include "types.hpp"
#include "enums.hpp"

// Generation functions
string addMissingZeros(string);
string addMissingZerosToNumber(string);
string prepareSymbol(string, string);
string prepareLocatorsSize(node*);
void initializeWithInstruction(string);
void generateDeclaration(node*, int);
string lookInSmybolToNumericTable(string, string);
string operationOpcodeMap(node*);
vector < string > lookForSymbolRow(string, string);
string findLocatorsPosition(string, node*);
void generateExpr(node*, int);
string lookForFunctionLabel(string);
void generateFunctionCall(node*, string);
void generateAssignment(node*);
void generateStatements(node*);
void generateComparison(string, int, string, string);
void generateCondition(node*, int);
string generateElif(node*, string);
void generateElse(node*);
void generateSelection(node*);
void generateLoop(node*);
void generateReturn(node*);
void generateFunction(node*);
void generateStartFunction(node*);
void generateProgram(node*);
void generateCode(node*, int);

#endif