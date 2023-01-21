#ifndef PARSER_STATIC_SEMATICS_H
#define PARSER_STATIC_SEMATICS_H

#include <iostream>

#include <fstream>

#include "types.hpp"

#include "enums.hpp"

using namespace std;

// Parsing functions
vector < node * > parseAndStaticSemantics();
void worldInitialization(node * parent, node * parentAST, int optionality);
void initialization(node * parent, node * parentAST, int optionality);
void initFunctions(node * parent, node * parentAST, int optionality);
void program(node * parent, node * parentAST, int optionality);
void startFunction(node * parent, node * parentAST, int optionality);
void _function(node * parent, node * parentAST, int optionality);
void statements(node * parent, node * parentAST, int optionality);
void types(node * parent, node * parentAST, int optionality);
void returned(node * parent, node * parentAST, int optionality);
void intParams(node * parent, node * parentAST, int optionality);
void otherIntParams(node * parent, node * parentAST, int optionality);
void intEndParam(node * parent, node * parentAST, int optionality);
void paramsWithoutTypes(node * parent, node * parentAST, int optionality);
void otherParamsWithoutTypes(node * parent, node * parentAST, int optionality);
void statements(node * parent, node * parentAST, int optionality);
void statement(node * parent, node * parentAST, int optionality);
void assignOrFctCallWithoutId(node * parent, node * parentAST);
void loop(node * parent, node * parentAST, int optionality);
void conditions(node * parent, node * parentAST, int optionality);
void condition1(node * parent, node * parentAST, int optionality);
void condition2(node * parent, node * parentAST, int optionality);
void condition3(node * parent, node * parentAST, int optionality);
void condition(node * parent, int optionality);
void expr(node * parent, node * parentAST, int optionality);
void term(node * parent, node * parentAST, int optionality);
void factor(node * parent, node * parentAST, int optionality);
void numIdFunc(node * parent, node * parentAST, int optionality);
void selection(node * parent, node * parentAST, int optionality);
void elifs(node * parent, node * parentAST, int optionality);
void elseStatement(node * parent, node * parentAST, int optionality);
void assignmentWithoutId(node * parent, node * parentAST, int optionality);
void assignmentForPicker(node * parent, node * parentAST, int optionality);
void locators(node * parent, node * parentAST, int optionality);
void numId(node * parent, node * parentAST, int optionality);
void declaration(node * parent, node * parentAST, int optionality);
void functionCallWithoutId(node * parent, node * parentAST, int optionality);
void builtInFunctionCall(node * parent, node * parentAST, int optionality);
void pickerWithLocators(node * parent, node * parentAST, int optionality);
void otherParamsWithTypes(node * parent, node * parentAST, int optionality);
void paramsWithTypes(node * parent, node * parentAST, int optionality);

#endif