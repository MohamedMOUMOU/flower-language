#ifndef ENUMS_H
#define ENUMS_H

#include <string>
#include <vector>

// Declaring string vecrors for each terminal in our grammar
const vector < string > NUM = {
    "NUM",
    "number"
};
const vector < string > PICKER = {
    "PICKER",
    "PICKER"
};
const vector < string > ELSE = {
    "ELSE",
    "else"
};
const vector < string > LCBR = {
    "LCBR",
    "{"
};
const vector < string > RCBR = {
    "RCBR",
    "}"
};
const vector < string > LSQB = {
    "LSQB",
    "["
};
const vector < string > RSQB = {
    "RSQB",
    "]"
};
const vector < string > BUILTIN = {
    "BUILTIN",
    "buil-tin function"
};
const vector < string > LPAR = {
    "LPAR",
    "("
};
const vector < string > RPAR = {
    "RPAR",
    ")"
};
const vector < string > INT = {
    "INT",
    "integer"
};
const vector < string > ID = {
    "ID",
    "identifier"
};
const vector < string > ELIF = {
    "ELIF",
    "elif"
};
const vector < string > IF = {
    "IF",
    "if"
};
const vector < string > ASSIGNMENT = {
    "ASSIGNMENT",
    "="
};
const vector < string > PLUSMINUS = {
    "PLUSMINUS",
    "+ or -"
};
const vector < string > TIMESDIVIDE = {
    "TIMESDIVIDE",
    "* or /"
};
const vector < string > RELATION = {
    "RELATION",
    "==, >, >=, < or <="
};
const vector < string > NOT = {
    "NOT",
    "not"
};
const vector < string > OR = {
    "OR",
    "or"
};
const vector < string > AND = {
    "AND",
    "and"
};
const vector < string > SEMICOLON = {
    "SEMICOLON",
    ";"
};
const vector < string > COMMA = {
    "COMMA",
    ","
};
const vector < string > INITFUNC = {
    "INITFUNCTIONS",
    "a world initialization function"
};
const vector < string > QUESTIONMARK = {
    "QUESTIONMARK",
    "?"
};
const vector < string > RETURN = {
    "RETURN",
    "return"
};
const vector < string > START = {
    "START",
    "start"
};
const vector < string > VOID = {
    "VOID",
    "void"
};
const vector < string > BUILDWORLD = {
    "BUILDWORLD",
    "buildworld"
};
const vector < string > EMPTYWORLD = {
    "EMPTYWORLD",
    "emptyWorld"
};
const vector < string > FUNC = {
    "FUNC",
    "function"
};
const vector < string > WORLDBUILT = {
    "WORLDBUILT",
    "worldbuilt"
};
const vector < string > SKIP = {
    "SKIP",
    "skip"
};
const vector < string > BREAK = {
    "BREAK",
    "break"
};
const vector < string > WHILE = {
    "WHILE",
    "while"
};

// Declaring strings corresponding to the non-terminals of our grammar
const string NUMLOCATORS = "NUMLOCATORS";
const string STATEMENTS = "STATEMENTS";
const string STATEMENT = "STATEMENT";
const string SELECTION = "SELECTION";
const string LOOP = "LOOP";
const string CONDITIONS = "CONDITIONS";
const string ELIFS = "ELIFS";
const string ELSESTATEMENT = "ELSESTATEMENT";
const string EXPR = "EXPR";
const string BUILTINFUNCTIONCALL = "BUILTINFUNCTIONCALL";
const string FUNCTIONCALLWITHOUTID = "FUNCTIONCALLWITHOUTID";
const string TERM = "TERM";
const string FACTOR = "FACTOR";
const string NUMIDFUNC = "NUMIDFUNC";
const string NUMID = "NUMID";
const string CONDITION = "CONDITION";
const string CONDITION1 = "CONDITION";
const string CONDITION2 = "CONDITION2";
const string CONDITION3 = "CONDITION3";
const string OTHERPARAMSWITHOUTTYPES = "OTHERPARAMSWITHOUTTYPES";
const string ENDPARAMWITHOUTTYPES = "ENDPARAMWITHOUTTYPES";
const string OTHERINTPARAMS = "OTHERINTPARAMS";
const string INTENDPARAM = "INTENDPARAM";
const string INITFUNCTIONS = "INITFUNCTIONSNONTERNMINAL";
const string INITIALIZATION = "INITIALIZATION";
const string WORLDINITIALIZATION = "WORLDINITIALIZATION";
const string PROGRAM = "PROGRAM";
const string CODE = "CODE";
const string FUNCTION = "FUNCTION";
const string DECLARATION = "DECLARATION";
const string STARTFUNCTION = "STARTFUNCTION";
const string TYPES = "TYPES";
const string PARAMSWITHTYPES = "PARAMSWITHTYPES";
const string PARAMSWITHOUTTYPES = "PARAMSWITHOUTTYPES";
const string RETURNED = "RETURNED";
const string INTPARAMS = "INTPARAMS";
const string ASSIGNORFCTCALLWITHOUTID = "ASSIGNORFCTCALLWITHOUTID";
const string ASSIGNMENTFORPICKER = "ASSIGNMENTFORPICKER";
const string ASSIGNMENTWITHOUTID = "ASSIGNMENTWITHOUTID";
const string LOCATORS = "LOCATORS";
const string PICKERWITHLOCATORS = "PICKERWITHLOCATORS";
const string OTHERPARAMSWITHTYPES = "OTHERPARAMSWITHTYPES";
const string ENDPARAMSWITHTYPES = "ENDPARAMSWITHTYPES";
const string ENDPARAMWITHTYPES = "ENDPARAMWITHTYPES";
const string COMPOUNDSTATEMENTS = "COMPOUNDSTATEMENTS";
const string FUNCTIONDEFINITION = "FUNCTIONDEFINITION";
const string FUNCTIONCALL = "FUNCTIONCALL";
const string PARAMS = "PARAMS";
const string GLOBAL = "GLOBAL";

#endif