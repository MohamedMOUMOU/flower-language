#include <iostream>

#include <fstream>

#include <string>

#include <vector>

#include <iomanip>

#define VERBOSE 1

using namespace std;

struct node {
    vector < string > value;
    vector < node * > children;
};

ifstream lexerOutput;

// Declaring the protypes of all the functions corresponding to the rules of our
// grammar
vector < node * > code();
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

// Declaring string vecrors for each terminal in our grammar
vector < string > NUM = {
    "NUM",
    "number"
};
vector < string > PICKER = {
    "PICKER",
    "PICKER"
};
vector < string > ELSE = {
    "ELSE",
    "else"
};
vector < string > LCBR = {
    "LCBR",
    "{"
};
vector < string > RCBR = {
    "RCBR",
    "}"
};
vector < string > LSQB = {
    "LSQB",
    "["
};
vector < string > RSQB = {
    "RSQB",
    "]"
};
vector < string > BUILTIN = {
    "BUILTIN",
    "buil-tin function"
};
vector < string > LPAR = {
    "LPAR",
    "("
};
vector < string > RPAR = {
    "RPAR",
    ")"
};
vector < string > INT = {
    "INT",
    "integer"
};
vector < string > ID = {
    "ID",
    "identifier"
};
vector < string > ELIF = {
    "ELIF",
    "elif"
};
vector < string > IF = {
    "IF",
    "if"
};
vector < string > ASSIGNMENT = {
    "ASSIGNMENT",
    "="
};
vector < string > PLUSMINUS = {
    "PLUSMINUS",
    "+ or -"
};
vector < string > TIMESDIVIDE = {
    "TIMESDIVIDE",
    "* or /"
};
vector < string > RELATION = {
    "RELATION",
    "==, >, >=, < or <="
};
vector < string > NOT = {
    "NOT",
    "not"
};
vector < string > OR = {
    "OR",
    "or"
};
vector < string > AND = {
    "AND",
    "and"
};
vector < string > SEMICOLON = {
    "SEMICOLON",
    ";"
};
vector < string > COMMA = {
    "COMMA",
    ","
};
vector < string > INITFUNC = {
    "INITFUNCTIONS",
    "a world initialization function"
};
vector < string > QUESTIONMARK = {
    "QUESTIONMARK",
    "?"
};
vector < string > RETURN = {
    "RETURN",
    "return"
};
vector < string > START = {
    "START",
    "start"
};
vector < string > VOID = {
    "VOID",
    "void"
};
vector < string > BUILDWORLD = {
    "BUILDWORLD",
    "buildworld"
};
vector < string > EMPTYWORLD = {
    "EMPTYWORLD",
    "emptyWorld"
};
vector < string > FUNC = {
    "FUNC",
    "function"
};
vector < string > WORLDBUILT = {
    "WORLDBUILT",
    "worldbuilt"
};
vector < string > SKIP = {
    "SKIP",
    "skip"
};
vector < string > BREAK = {
    "BREAK",
    "break"
};
vector < string > WHILE = {
    "WHILE",
    "while"
};

// Declaring strings corresponding to the non-terminals of our grammar
string NUMLOCATORS = "NUMLOCATORS";
string STATEMENTS = "STATEMENTS";
string STATEMENT = "STATEMENT";
string SELECTION = "SELECTION";
string LOOP = "LOOP";
string CONDITIONS = "CONDITIONS";
string ELIFS = "ELIFS";
string ELSESTATEMENT = "ELSESTATEMENT";
string EXPR = "EXPR";
string BUILTINFUNCTIONCALL = "BUILTINFUNCTIONCALL";
string FUNCTIONCALLWITHOUTID = "FUNCTIONCALLWITHOUTID";
string TERM = "TERM";
string FACTOR = "FACTOR";
string NUMIDFUNC = "NUMIDFUNC";
string NUMID = "NUMID";
string CONDITION = "CONDITION";
string CONDITION1 = "CONDITION";
string CONDITION2 = "CONDITION2";
string CONDITION3 = "CONDITION3";
string OTHERPARAMSWITHOUTTYPES = "OTHERPARAMSWITHOUTTYPES";
string ENDPARAMWITHOUTTYPES = "ENDPARAMWITHOUTTYPES";
string OTHERINTPARAMS = "OTHERINTPARAMS";
string INTENDPARAM = "INTENDPARAM";
string INITFUNCTIONS = "INITFUNCTIONSNONTERNMINAL";
string INITIALIZATION = "INITIALIZATION";
string WORLDINITIALIZATION = "WORLDINITIALIZATION";
string PROGRAM = "PROGRAM";
string CODE = "CODE";
string FUNCTION = "FUNCTION";
string DECLARATION = "DECLARATION";
string STARTFUNCTION = "STARTFUNCTION";
string TYPES = "TYPES";
string PARAMSWITHTYPES = "PARAMSWITHTYPES";
string PARAMSWITHOUTTYPES = "PARAMSWITHOUTTYPES";
string RETURNED = "RETURNED";
string INTPARAMS = "INTPARAMS";
string ASSIGNORFCTCALLWITHOUTID = "ASSIGNORFCTCALLWITHOUTID";
string ASSIGNMENTFORPICKER = "ASSIGNMENTFORPICKER";
string ASSIGNMENTWITHOUTID = "ASSIGNMENTWITHOUTID";
string LOCATORS = "LOCATORS";
string PICKERWITHLOCATORS = "PICKERWITHLOCATORS";
string OTHERPARAMSWITHTYPES = "OTHERPARAMSWITHTYPES";
string ENDPARAMSWITHTYPES = "ENDPARAMSWITHTYPES";
string ENDPARAMWITHTYPES = "ENDPARAMWITHTYPES";
string COMPOUNDSTATEMENTS = "COMPOUNDSTATEMENTS";
string FUNCTIONDEFINITION = "FUNCTIONDEFINITION";
string FUNCTIONCALL = "FUNCTIONCALL";
string PARAMS = "PARAMS";
string GLOBAL = "GLOBAL";

// Declaring the global variables in our program
vector < vector < string >> symbolTable;
int readingBackwardsFlag = 0;
vector < string > currentToken = {};
string currentFunctionScope = GLOBAL;
int nParamsFunction = 0;
int nParamsFunctionCall = 0;
string currentId;
int row = 1;
int column = 1;
int isArray = 0;
int functionDefinitionParam = 0;

// Declaring generation variables
vector < string > DATASECTION;
vector < string > CODESECTION;
string SP = "002";
string EP = "003";
string temp1 = "004";
string temp2 = "005";
string temp3 = "006";
string temp4 = "007";
string tempReturn = "008";
string picker = "009";
int symbolsAssigned = 11;
int labelsAssigned = 1;
int SPSTART = symbolsAssigned;
vector < vector < string >> functionNameToLabel;
string constant0 = "+0000000000";
string currentFunctionName = "";

using namespace std;
// This function prints the information where an error occured. It prints the
// line number, the expected token and the token we got instead.
string staticSemanticsErrorMessage(string s) {
    return "Error: " + s;
}

// Defining the expected token exception tha should be thrown when the parser
// gets a different token than expected
class expectedToken: public std::exception {
    private: string message;

    public: expectedToken(string msg): message(msg) {}
    string what() {
        return message;
    }
};

// Defining the catchableExpectedToken Exception which is thrown whenever we try
// a token and soesn't work mainly when we have an "or" or an optional token
class catchableExpectedToken: public std::exception {
    private: string message;

    public: catchableExpectedToken(string msg): message(msg) {}
    string what() {
        return message;
    }
};

// Defining an exception for static semantics errors
class staticSemanticsException: public std::exception {
    private: string message;

    public: staticSemanticsException(string msg): message(msg) {}
    string what() {
        return message;
    }
};

// This function prints the information where an error occured. It prints the
// line number, the expected token and the token we got instead.
string errorMessage(string s1, string s2, string s3) {
    return "Line: " + s1 + " - Expected " + s2 + ", got " + s3 + "\n";
}

// Function defined to generate spaces while printing the trees
string spaces(int level) {
    string s = "";
    while (level != 0) {
        s += " ";
        level--;
    }
    return s;
}

// Function used to add an entry to the symbol table in which we say that a
// variable was declared
void declareVar(string name) {
    if (isArray)
        symbolTable.push_back({
            name,
            "VARIABLE",
            "1",
            to_string(row),
            currentFunctionScope,
            to_string(column)
        });
    else {
        symbolTable.push_back({
            name,
            "VARIABLE",
            "1",
            "0",
            currentFunctionScope,
            "0"
        });
    }
    isArray = 0; // reset
}

// Function used to add an entry to the symbol table in which we say that a
// function is declared
void defineFunc(string name) {
    symbolTable.push_back({
        name,
        "SUBPROGRAM",
        "1",
        "1",
        GLOBAL,
        to_string(nParamsFunction)
    });
}

// Function used to add an entry to the symbol table in which we say that a
// variable is declare
void defineVar(string name) {
    int defined = 0;
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == name && symbolTable[i][1] == "VARIABLE" &&
            symbolTable[i][4] == currentFunctionScope) {
            symbolTable[i][3] = "1";
            defined = 1;
            break;
        }
    }
    if (!defined) {
        for (int i = 0; i < symbolTable.size(); i++) {
            if (symbolTable[i][0] == name && symbolTable[i][1] == "VARIABLE" &&
                symbolTable[i][4] == GLOBAL) {
                symbolTable[i][3] = "1";
                break;
            }
        }
    }
}

// This function checks if a variable is declared and throws a static semantic
// exception in the case where the variable is not declared
void checkVarIfDeclared(string name) {
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == name && symbolTable[i][1] == "VARIABLE" &&
            (symbolTable[i][4] == currentFunctionScope ||
                symbolTable[i][4] == GLOBAL))
            return;
    }
    throw staticSemanticsException(
        staticSemanticsErrorMessage("In line: " + currentToken[0] + " Variable not declared -> " + name));
}

// This function checks if a function is defined and throws a static semantic
// exception in the case where the function is not defined
void checkFuncIfDefined(string name) {
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == name && symbolTable[i][1] == "SUBPROGRAM") {
            return;
        }
    }
    throw staticSemanticsException(
        staticSemanticsErrorMessage("In line: " + currentToken[0] + " Function not defined -> " + name));
}

// This function checks whether the number of parameters in the function call is
// the same as the number of parameters in the function definition. It throws an
// exception if not the same
void checkParamNumber(string name) {
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == name && symbolTable[i][1] == "SUBPROGRAM") {
            if (symbolTable[i][5] != to_string(nParamsFunctionCall)) {
                string error = "Expected " + symbolTable[i][5] + ", got " +
                    to_string(nParamsFunctionCall);
                throw staticSemanticsException(staticSemanticsErrorMessage(error));
            }
        }
    }
}

// This function checks if a variable is defined and throws a static semantic
// exception in the case where the variable is not defined
void checkVarDefined(vector < string > token) {
    if (token[2] == BUILTIN[0] || token[3] == "picker")
        return;
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == token[3] && symbolTable[i][1] == "VARIABLE" &&
            (symbolTable[i][4] == currentFunctionScope ||
                symbolTable[i][4] == GLOBAL) &&
            stoi(symbolTable[i][3]) >= 1)
            return;
    }
    throw staticSemanticsException(
        staticSemanticsErrorMessage("In line: " + currentToken[0] + " Variable not initialized -> " + token[3]));
}

ofstream output;

int spaceToAdd = 0;
void printTree(node * root, int abst) {
    if (root -> value.size() > 1 && abst) {
        if (VERBOSE) cout << root -> value[1];
        output << root -> value[1];
    } else {
        if (VERBOSE)
            cout << root -> value[0];
        output << root -> value[0];
    }
    int i = 0;
    for (node * child: root -> children) {
        if (i == 0)
            spaceToAdd += ((root -> value.size() > 1 ? root -> value[1].size() :
                    root -> value[0].size()) +
                3);
        else {

            if (VERBOSE)
                cout << "\n" << spaces(spaceToAdd - 3);
            output << "\n" << spaces(spaceToAdd - 3);
        }
        if (VERBOSE)
            cout << " - ";
        output << " - ";
        printTree(child, abst);
        i++;
    }
    if (root -> children.size() != 0)
        spaceToAdd -= ((root -> value.size() > 1 ? root -> value[1].size() :
                root -> value[0].size()) +
            3);
}

void printSymbolTable() {
    cout << "--------------------------- SYMBOL TABLE ---------------------------\n";
    cout << '|' << setw(10) << "Name" <<
        '|' << setw(10) << "Type" <<
        '|' << setw(10) << "Declared" <<
        '|' << setw(10) << "Defined" <<
        '|' << setw(10) << "Scope" <<
        '|' << setw(11) << "# of params" <<
        '|' << endl;
    cout << "--------------------------------------------------------------------\n";
    for (int i = 0; i < symbolTable.size(); i++) {
        cout << '|' << setw(10) << symbolTable[i][0] <<
            '|' << setw(10) << symbolTable[i][1] <<
            '|' << setw(10) << symbolTable[i][2] <<
            '|' << setw(10) << symbolTable[i][3] <<
            '|' << setw(10) << symbolTable[i][4] <<
            '|' << setw(11) << symbolTable[i][5] <<
            '|' << endl;
    }
}

void stdioAndFileOutput(node * headCST, node * headAST) {
    output.open("concreteSyntaxTree.txt");
    if (VERBOSE) cout << "CONCRETE SYNTAX TREE\n";
    printTree(headCST, 0);
    output.close();
    output.open("abstractSyntaxTree.txt");
    if (VERBOSE) cout << "CONCRETE SYNTAX TREE\n";
    printTree(headAST, 1);
    output.close();
    if (VERBOSE) printSymbolTable();
    output.open("generated.txt");
    if (VERBOSE) cout << "GENERATED CODE\n";
    if (VERBOSE) cout << "DATASECTION\n";
    for (string s: DATASECTION) {
        if (VERBOSE) cout << s << "\n";
        output << s << "\n";
    }
    if (VERBOSE) cout << "CODE SECTION\n";
    for (string s: CODESECTION) {
        if (VERBOSE) cout << s << "\n";
        output << s << "\n";
    }
    output.close();
}

// This function get the informaion of a token given the line. It is used in
// nextToken() function
vector < string > getTokenInfo(string line) {
    string space_delimiter = " ";
    vector < string > words = {};

    size_t pos = 0;
    while ((pos = line.find(space_delimiter)) != string::npos) {
        words.push_back(line.substr(0, pos));
        line.erase(0, pos + space_delimiter.length());
    }
    words.push_back(line.substr(0, line.size()));
    vector < string > info = {
        words[0],
        words[1],
        words[2]
    };
    if (words.size() > 3)
        info.push_back(words[3]);
    return info;
}

// This function returns the next token following the current token
vector < string > nextToken() {
    // takes lexerOutput as the file from where to read input
    if (readingBackwardsFlag) {
        readingBackwardsFlag = 0;
        return currentToken;
    }
    string Line;
    getline(lexerOutput, Line);
    vector < string > token = getTokenInfo(Line);
    currentToken = token;
    return token;
}

// This function creates a node and append it to the children vector of the
// parent
node * createAppendNode(node * parent, vector < string > token) {
    vector < string > tokenToStore = {};
    tokenToStore.push_back(token[2]);
    if (token.size() > 3)
        tokenToStore.push_back(token[3]);
    node * node1 = new node;
    node1 -> value = tokenToStore;
    parent -> children.push_back(node1);
    return node1;
}

// This function processes the token by either creating the corresponding node
// and appending it to the vector, throw catchableExpectedToken if the token
// might be optional, and throw expectedToken if the token is required but got a
// different token
void tokenProcessing(node * parent, vector < string > token, int catchable) {
    vector < string > token1 = nextToken();
    if (token1[2] == token[0]) {
        createAppendNode(parent, token1);
    } else {
        if (catchable)
            throw catchableExpectedToken(
                errorMessage(token1[0], token[0], token1[2]));
        else
            throw expectedToken(errorMessage(token1[0], token[0], token1[2]));
    }
}

node * buildASTTerminal(node * parent, vector < string > token, vector < string > token1) {
    if (token1[2] == token[0]) {
        return createAppendNode(parent, token1);
    }
    return nullptr;
}

// This function processes the token but without throwing exceptions in the case
// of optional tokens or tokens where we have Or in teh RHS
int tokenProcessingWithoutErrorHandling(node * parent, vector < string > token) {
    vector < string > token1 = nextToken();
    if (token1[2] == token[0]) {
        createAppendNode(parent, token1);
        return 1;
    }
    readingBackwardsFlag = 1;
    currentToken = token1;
    return 0;
}

// This function creates a node with giving its value
node * createNodeWithValue(node * parent, string nonTerminalName) {
    node * node1 = new node;
    node1 -> value = {
        nonTerminalName
    };
    parent -> children.push_back(node1);
    return node1;
}

node * createNodeWithValueAST(node * parent, string nonTerminalName) {
    node * node1 = new node;
    node1 -> value = {
        nonTerminalName
    };
    parent -> children.push_back(node1);
    return node1;
}
// This function is used when we have to use pickerWithLocators subprogram
//<PICKER>->PICKER LSQB NUM RSQB
void pickerWithLocators(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, PICKER, optionality);
    node * pickerParent = createNodeWithValueAST(parentAST, PICKER[0]);
    tokenProcessing(parent, LSQB, 0);
    tokenProcessing(parent, NUM, 0);
    buildASTTerminal(pickerParent, NUM, currentToken);
    tokenProcessing(parent, RSQB, 0);
}

// This function is used when we have to use initFunctions subprogram
// <INIT FUNCTIONS> -> INITFUNC LPAR NUM COMMA NUM [COMMA NUM COMMA NUM] RPAR
void initFunctions(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, INITFUNC, optionality);
    tokenProcessing(parent, LPAR, 0);
    paramsWithoutTypes(createNodeWithValue(parent, PARAMSWITHOUTTYPES), createNodeWithValueAST(parentAST, PARAMS), 0);
}
// This function is used when we have to use initialization subprogram
//<INITIALIZATION> -> <INIT FUNCTIONS> <INITIALIZATION> | Ꜫ
void initialization(node * parent, node * parentAST, int optionality) {
    while (nextToken()[2] != WORLDBUILT[0]) {
        readingBackwardsFlag = 1;
        node * initFuncParent = createNodeWithValueAST(parentAST, "INIFUNC");
        node * initFuncNameParent = buildASTTerminal(initFuncParent, INITFUNC, currentToken);
        initFunctions(createNodeWithValue(parent, INITFUNCTIONS), initFuncNameParent, optionality);
    }
    readingBackwardsFlag = 1;
}

// This function is used when we have to use statements subprogram
// <STATEMENTS> -> {<STATEMENT>}
void statements(node * parent, node * parentAST, int optionality) {
    while (nextToken()[2] != RCBR[0]) {
        readingBackwardsFlag = 1;
        statement(createNodeWithValue(parent, STATEMENT), parentAST, 0);
    }
    readingBackwardsFlag = 1;
}

// This function is used when we have to use program subprogram
//<PROGRAM> -> {<FUNCTION>|<DECLARATION> SEMICOLON} <START FUNCTION>
void program(node * parent, node * parentAST, int optionality) {
    while (nextToken()[2] != "START") {
        readingBackwardsFlag = 1;
        try {
            _function(createNodeWithValue(parent, FUNCTION), createNodeWithValueAST(parentAST, FUNCTION), 1);
        } catch (catchableExpectedToken e) {
            parent -> children.pop_back();
            parentAST -> children.pop_back();
            readingBackwardsFlag = 1;
            declaration(createNodeWithValue(parent, DECLARATION), createNodeWithValueAST(parentAST, DECLARATION), 0);
        }
    }
    readingBackwardsFlag = 1;
    currentToken = currentToken;
    startFunction(createNodeWithValue(parent, STARTFUNCTION), createNodeWithValueAST(parentAST, STARTFUNCTION), 0);
}

// This function is used when we have to use otherParamsWithoutTypes subprogram
//<OTHER PARAMS WITHOUT TYPES> -> (< NUMIDFUNC> | <PICKER>) (COMMA  <OTHER
// PARAMS WITHOUT TYPES> | RPAR)
void otherParamsWithoutTypes(node * parent, node * parentAST, int optionality) {
    int caught = 0;
    try {
        numIdFunc(createNodeWithValue(parent, NUMIDFUNC), parentAST, 1);
        nParamsFunctionCall += 1;
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        readingBackwardsFlag = 1;
        pickerWithLocators(createNodeWithValue(parent, PICKERWITHLOCATORS), parentAST,
            optionality);
        nParamsFunctionCall += 1;
    }
    if (!caught) {
        if (tokenProcessingWithoutErrorHandling(parent, RPAR)) {} else {
            tokenProcessing(parent, COMMA, 0);
            otherParamsWithoutTypes(
                createNodeWithValue(parent, OTHERPARAMSWITHOUTTYPES), parentAST, 0);
        }
    }
}

// This function is used when we have to use paramsWithoutTypes subprogram
//<PARAMS WITHOUT TYPES> -> [<OTHER PARAMS WITHOUT TYPES>]
// TYPE>
void paramsWithoutTypes(node * parent, node * parentAST, int optionality) {
    nParamsFunctionCall = 0;
    try {
        otherParamsWithoutTypes(createNodeWithValue(parent, OTHERPARAMSWITHOUTTYPES), parentAST, 1);
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        readingBackwardsFlag = 1;
        tokenProcessing(parent, RPAR, 0);
    }
}

// This function is used when we have to use assignmentWithoutId subprogram
// CHANGE IN GRAMMAR
//<ASSIGNMENT WITHOUT ID> -> [<LOCATORS>] ASSIGNMENT <expr>
void assignmentWithoutId(node * parent, node * parentAST, int optionality) {
    node * idParent = parentAST -> children[0];
    try {
        locators(createNodeWithValue(parent, LOCATORS), idParent, 1);
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        readingBackwardsFlag = 1;
    }
    tokenProcessing(parent, ASSIGNMENT, optionality);
    checkVarIfDeclared(currentId);
    if (!isArray) { // cuz if array ignore
        defineVar(currentId);
    }
    isArray = 0;
    expr(createNodeWithValue(parent, EXPR), parentAST, 0);
}

// This function is used when we have to use worldInitialization subprogram
//<WORLD INITIALIZATION> -> BUILDWORLD EMPTYWORLD LPAR NUM COMMA NUM RPAR
//<INITIALIZATION> WORLDBUILT
void worldInitialization(node * parent, node * parentAST, int optionality) {
    int caught = 0;
    try {
        tokenProcessing(parent, BUILDWORLD, optionality);
    } catch (catchableExpectedToken e) {
        caught = 1;
        readingBackwardsFlag = 1;
    }
    if (!caught) {
        tokenProcessing(parent, EMPTYWORLD, 0);
        node * emptyWorldParent = buildASTTerminal(parentAST, EMPTYWORLD, currentToken);
        tokenProcessing(parent, LPAR, 0);
        node * paramsParent = createNodeWithValueAST(emptyWorldParent, PARAMS);
        paramsWithoutTypes(createNodeWithValue(parent, PARAMSWITHOUTTYPES), paramsParent, 0);
        initialization(createNodeWithValue(parent, INITIALIZATION), parentAST, 1);
        tokenProcessing(parent, WORLDBUILT, 0);
    }
}

// This function is used when we have to use code subprogram
//<CODE> -> <WORLD INITIALIZATION><PROGRAM>
vector < node * > code() {
    node * parentCST = new node;
    parentCST -> value = {
        CODE
    };
    node * parentAST = new node;
    parentAST -> value = {
        CODE
    };
    worldInitialization(createNodeWithValue(parentCST, WORLDINITIALIZATION), parentAST, 0);
    program(createNodeWithValue(parentCST, PROGRAM), createNodeWithValueAST(parentAST, PROGRAM), 0);
    return {
        parentCST,
        parentAST
    };
}

// This function is used when we have to use types subprogram
//<TYPES>-> VOID | INT
void types(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, VOID)) {
        buildASTTerminal(parentAST, VOID, currentToken);
    } else {
        tokenProcessing(parent, INT, optionality);
        buildASTTerminal(parentAST, INT, currentToken);
    }
}

// This function is used when we have to use startFunction subprogram
//<START FUNCTION> -> START LPAR RPAR LCBR <STATEMENTS>RCBR
void startFunction(node * parent, node * parentAST, int optionality) {
    int caught = 0;
    try {
        tokenProcessing(parent, START, optionality);
    } catch (catchableExpectedToken e) {
        caught = 1;
        readingBackwardsFlag = 1;
    }
    if (!caught) {
        currentFunctionScope = START[0];
        tokenProcessing(parent, LPAR, 0);
        tokenProcessing(parent, RPAR, 0);
        tokenProcessing(parent, LCBR, 0);
        statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
        tokenProcessing(parent, RCBR, 0);
        currentFunctionScope = GLOBAL;
    }
}

// This function is used when we have to use function subprogram
//<FUNCTION> -> FUNC <TYPES> ID LPAR <PARAMS WITH TYPES> LCBR <statements> RCBR
void _function(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, FUNC, optionality);
    types(createNodeWithValue(parent, TYPES), parentAST, 0);
    tokenProcessing(parent, ID, 0);
    buildASTTerminal(parentAST, ID, currentToken);
    string nameFunction = currentToken[3];
    currentFunctionScope = nameFunction;
    functionDefinitionParam = 1;
    tokenProcessing(parent, LPAR, 0);
    // IT SHOULD BE WITH TYPES
    paramsWithTypes(createNodeWithValue(parent, PARAMSWITHTYPES), createNodeWithValueAST(parentAST, PARAMSWITHTYPES), 1);
    // inserting info about the function in the symbol table, after its definition
    defineFunc(nameFunction);
    tokenProcessing(parent, LCBR, 0);
    functionDefinitionParam = 0;
    statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
    tokenProcessing(parent, RCBR, 0);
    currentFunctionScope = GLOBAL;
    nParamsFunction = 0;
}

// This function is used when we have to use returned subprogram
//<RETURNED> -> < NUMID> SEMICOLON| Ꜫ
void returned(node * parent, node * parentAST, int optionality) {
    try {
        numId(createNodeWithValue(parent, NUMID), parentAST, 1);
        tokenProcessing(parent, SEMICOLON, 0);
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        readingBackwardsFlag = 1;
    }
}
//<INT PARAMS> -> [<OTHER INT PARAMS >] <INT ENDPARAM>
void intParams(node * parent, node * parentAST, int optionality) {
    try {
        otherIntParams(createNodeWithValue(parent, OTHERINTPARAMS), parentAST, 1);
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        readingBackwardsFlag = 1;
    }
    intEndParam(createNodeWithValue(parent, INTENDPARAM), parentAST, optionality);
}

// This function is used when we have to use otherIntParams subprogram
//<OTHER INT PARAMS> -> INT ID COMMA <OTHER INT PARAMS > | Ꜫ
void otherIntParams(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, INT)) {
        tokenProcessing(parent, ID, 0);
        buildASTTerminal(parentAST, ID, currentToken);
        tokenProcessing(parent, COMMA, 0);
        otherIntParams(createNodeWithValue(parent, OTHERINTPARAMS), parentAST, 0);
    }
}

// This function is used when we have to use intEndParam subprogram
//<INT ENDPARAM > -> INT ID | Ꜫ
void intEndParam(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, INT)) {
        buildASTTerminal(parentAST, ID, currentToken);
        tokenProcessing(parent, ID, 0);
    }
}

// This function is used when we have to use factor subprogram
//<FACTOR> -> <NUMID>| LPAR <EXPR> RPAR
void factor(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, LPAR)) {
        expr(createNodeWithValue(parent, EXPR), parentAST, 0);
        tokenProcessing(parent, RPAR, 0);
    } else {
        numIdFunc(createNodeWithValue(parent, NUMID), parentAST, optionality);
    }
}

// This function is used when we have to use term subprogram
//<TERM> -> <FACTOR> [ TIMESDIVIDE <TERM>]
void term(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * timesdivideParent = createNodeWithValueAST(parentAST, TIMESDIVIDE[0]);
    factor(createNodeWithValue(parent, FACTOR), timesdivideParent, optionality);
    if (tokenProcessingWithoutErrorHandling(parent, TIMESDIVIDE)) {
        timesdivideParent -> value.push_back(currentToken[3]);
        term(createNodeWithValue(parent, TERM), timesdivideParent, 0);
    } else {
        parentAST -> children[position] = timesdivideParent -> children[0];
    }
}

// This function is used when we have to use expr subprogram
//<EXPR> -> <TERM> [PLUSMINUS <EXPR>]
void expr(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * plusminusParent = createNodeWithValueAST(parentAST, PLUSMINUS[0]);
    term(createNodeWithValue(parent, TERM), plusminusParent, 0);
    if (tokenProcessingWithoutErrorHandling(parent, PLUSMINUS)) {
        plusminusParent -> value.push_back(currentToken[3]);
        expr(createNodeWithValue(parent, EXPR), plusminusParent, 0);
    } else {
        parentAST -> children[position] = plusminusParent -> children[0];
    }
}

// This function is used when we have to use condition subprogram
//<CONDITION> -> <EXPR> [RELATION <EXPR>]
void condition(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * relationParent = createNodeWithValueAST(parentAST, RELATION[0]);
    expr(createNodeWithValue(parent, EXPR), relationParent, optionality);
    if (tokenProcessingWithoutErrorHandling(parent, RELATION)) {
        relationParent -> value.push_back(currentToken[3]);
        expr(createNodeWithValue(parent, EXPR), relationParent, 0);
    } else {
        parentAST -> children[position] = relationParent -> children[0];
    }
}

// This function is used when we have to use condition3 subprogram
//<CONDITION3> -> <CONDITION> | QUESTIONMARK <CONDITIONS> QUESTIONMARK
void condition3(node * parent, node * parentAST, int optionality) {
    try {
        tokenProcessing(parent, QUESTIONMARK, 1);
        conditions(createNodeWithValue(parent, CONDITIONS), parentAST, 0);
        tokenProcessing(parent, QUESTIONMARK, 0);
    } catch (catchableExpectedToken e) {
        readingBackwardsFlag = 1;
        condition(createNodeWithValue(parent, CONDITION), parentAST, 0);
    }
}

// This function is used when we have to use condition2 subprogram
//<CONDITION2> -> [NOT] <CONDITION3>
void condition2(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * notParent = createNodeWithValueAST(parentAST, NOT[0]);
    node * chosenParent = parentAST;
    if (tokenProcessingWithoutErrorHandling(parent, NOT)) {
        chosenParent = notParent;
    } else {
        parentAST -> children.pop_back();
    }
    condition3(createNodeWithValue(parent, CONDITION3), chosenParent, optionality);
}

// This function is used when we have to use condition1 subprogram
//<CONDITION1> -> <CONDITION2> [OR <CONDITION1>]
void condition1(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * orParent = createNodeWithValueAST(parentAST, OR[0]);
    condition2(createNodeWithValue(parent, CONDITION2), orParent, optionality);
    if (tokenProcessingWithoutErrorHandling(parent, OR)) {
        condition1(createNodeWithValue(parent, CONDITION1), orParent, optionality);
    } else {
        parentAST -> children[position] = orParent -> children[0];
    }
}

// This function is used when we have to use conditions subprogram
//<CONDITIONS> -> <CONDITION1> [AND <CONDITIONS>]
void conditions(node * parent, node * parentAST, int optionality) {
    int position = parentAST -> children.size();
    node * addParent = createNodeWithValueAST(parentAST, AND[0]);
    condition1(createNodeWithValue(parent, CONDITION1), addParent, optionality);
    if (tokenProcessingWithoutErrorHandling(parent, AND)) {
        conditions(createNodeWithValue(parent, CONDITIONS), addParent, optionality);
    } else {
        parentAST -> children[position] = addParent -> children[0];
    }
}

// This function is used when we have to use numIdFunc subprogram
//<NUMIDFUNC> -> NUM | ID [<FUNCTION CALL WITHOUT ID> | <locators>] |
//<BUILTINFUNCTION CALL>
void numIdFunc(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, NUM)) {
        buildASTTerminal(parentAST, NUM, currentToken);
    } else if (tokenProcessingWithoutErrorHandling(parent, ID)) {
        vector < string > savedID = currentToken;
        try {
            node * functionCallParent = createNodeWithValueAST(parentAST, FUNCTIONCALL);
            buildASTTerminal(functionCallParent, ID, savedID);
            functionCallWithoutId(createNodeWithValue(parent, FUNCTIONCALLWITHOUTID), functionCallParent,
                1);
        } catch (catchableExpectedToken e) {
            readingBackwardsFlag = 1;
            parent -> children.pop_back();
            parentAST -> children.pop_back();
            try {
                checkVarDefined(savedID);
                node * idParent = buildASTTerminal(parentAST, ID, savedID);
                locators(createNodeWithValue(parent, LOCATORS), idParent, 1);
            } catch (catchableExpectedToken e) {
                readingBackwardsFlag = 1;
                parent -> children.pop_back();
            }
        }
    } else {
        node * builtinParent = createNodeWithValueAST(parentAST, FUNCTIONCALL);
        node * builtinFunctionNameParent = buildASTTerminal(builtinParent, BUILTIN, currentToken);
        builtInFunctionCall(createNodeWithValue(parent, BUILTINFUNCTIONCALL), builtinParent,
            optionality);
    }
}

// This function is used when we have to use elifs subprogram
//<ELIFS> -> ELIF LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR  <ELIFS>| Ꜫ
void elifs(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, ELIF)) {
        tokenProcessing(parent, LPAR, 0);
        conditions(createNodeWithValue(parent, CONDITIONS), parentAST, 0);
        tokenProcessing(parent, RPAR, 0);
        tokenProcessing(parent, LCBR, 0);
        statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
    } else {
        throw catchableExpectedToken(
            errorMessage(currentToken[0], currentToken[0], currentToken[2]));
    }
}

// This function is used when we have to use elseStatement subprogram
//<ELSESTATEMENT> -> ELSE LCBR <STATEMENTS> RCBR
void elseStatement(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, ELSE, optionality);
    tokenProcessing(parent, LCBR, 0);
    statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
    tokenProcessing(parent, RCBR, 0);
}

// This function is used when we have to use selection subprogram
//<SELECTION> -> IF LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR  [<ELIFS>]
//[<ELSESTATEMENT>]
void selection(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, IF, optionality);
    tokenProcessing(parent, LPAR, 0);
    conditions(createNodeWithValue(parent, CONDITIONS), parentAST, 0);
    tokenProcessing(parent, RPAR, 0);
    tokenProcessing(parent, LCBR, 0);
    statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
    tokenProcessing(parent, RCBR, 0);
    try {
        elifs(createNodeWithValue(parent, ELIFS), createNodeWithValueAST(parentAST, ELIF[0]), 1);
    } catch (catchableExpectedToken e) {
        readingBackwardsFlag = 1;
        parent -> children.pop_back();
        parentAST -> children.pop_back();
    }
    try {
        elseStatement(createNodeWithValue(parent, ELSESTATEMENT), createNodeWithValueAST(parentAST, ELSE[0]), 1);
    } catch (catchableExpectedToken e) {
        readingBackwardsFlag = 1;
        parent -> children.pop_back();
        parentAST -> children.pop_back();
    }
}

// This function is used when we have to use numId subprogram
//<NUMID> -> NUM | ID
void numId(node * parent, node * parentAST, int optionality) {
    if (tokenProcessingWithoutErrorHandling(parent, NUM)) {
        buildASTTerminal(parentAST, NUM, currentToken);
    } else if (tokenProcessingWithoutErrorHandling(parent, ID)) {
        buildASTTerminal(parentAST, ID, currentToken);
        if (functionDefinitionParam) {
            declareVar(currentToken[3]);
            defineVar(currentToken[3]);
        }
        checkVarDefined(currentToken);
    } else if (!optionality) {
        string s1 = NUM[1] + "/" + ID[1];
        throw expectedToken(errorMessage(currentToken[0], s1, currentToken[2]));
    }
}

// This function is used when we have to use locators subprogram
//<LOCATORS> ->LSQB <NUMID> RSQB [ LSQB <NUMID> RSQB]
void locators(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, LSQB, optionality);
    isArray = 1;
    node * locatorsParent = createNodeWithValueAST(parentAST, LOCATORS);
    numId(createNodeWithValue(parent, NUMID), locatorsParent, 0);
    tokenProcessing(parent, RSQB, 0);
    if (tokenProcessingWithoutErrorHandling(parent, LSQB)) {
        numId(createNodeWithValue(parent, NUMID), locatorsParent, 0);
        tokenProcessing(parent, RSQB, 0);
    }
}

// This function is used when we have to use assignmentForPicker subprogram
//<ASSIGNMENTFORPICKER> -> <PICKER> ASSIGNMENT (<NUMIDFUNC> | <PICKER>)
void assignmentForPicker(node * parent, node * parentAST, int optionality) {
    pickerWithLocators(createNodeWithValue(parent, PICKERWITHLOCATORS), parentAST,
        optionality);
    tokenProcessing(parent, ASSIGNMENT, 0);
    try {
        numIdFunc(createNodeWithValue(parent, NUMIDFUNC), parentAST, 1);
    } catch (catchableExpectedToken e) {
        readingBackwardsFlag = 1;
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        pickerWithLocators(createNodeWithValue(parent, PICKERWITHLOCATORS), parentAST, 0);
    }
}

// This function is used when we have to use loop subprogram
//<LOOP> -> WHILE LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR
void loop(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, WHILE, 1);
    tokenProcessing(parent, LPAR, 0);
    conditions(createNodeWithValue(parent, CONDITIONS), parentAST, 0);
    tokenProcessing(parent, RPAR, 0);
    tokenProcessing(parent, LCBR, 0);
    statements(createNodeWithValue(parent, STATEMENTS), createNodeWithValueAST(parentAST, STATEMENTS), 0);
    tokenProcessing(parent, RCBR, 0);
}

// This function is used when we have to use assignOrFctCallWithoutId subprogram
//<ASSIGNORFCTCALLWITHOUTID> -><FUNCTIONCALLWITHOUTID>|<ASSIGNMENT WITHOUT ID>
void assignOrFctCallWithoutId(node * parent, node * parentAST, int optionality) {
    vector < string > idToken = currentToken;
    node * functionCallParent = createNodeWithValueAST(parentAST, FUNCTIONCALL);
    try {
        buildASTTerminal(functionCallParent, ID, idToken);
        functionCallWithoutId(createNodeWithValue(parent, FUNCTIONCALLWITHOUTID), functionCallParent,
            1);
    } catch (catchableExpectedToken e) {
        readingBackwardsFlag = 1;
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        node * assignmentParent = createNodeWithValueAST(parentAST, ASSIGNMENT[0]);
        buildASTTerminal(assignmentParent, ID, idToken);
        assignmentWithoutId(createNodeWithValue(parent, ASSIGNMENTWITHOUTID), assignmentParent,
            optionality);
        if (!optionality) {
            string s1 = ASSIGNMENT[1] + " or " + LPAR[1] + " ";
            throw expectedToken(errorMessage(currentToken[0], s1, currentToken[2]));
        }
    }
}

// This function is used when we have to use statement subprogram
//<STATEMENT> ->
//<SELECTION>|<LOOP>|ID<ASSIGNORFCTCALLWITHOUTID>SEMICOLON|<BUILTINFUNCTIONCALL>
// SEMICOLON|  <DECLARATION>|BREAK SEMICOLON|SKIP
// SEMICOLON|<ASSIGNMENTFORPICKER> SEMICOLON | RETURN <RETURNED>
void statement(node * parent, node * parentAST, int optionality) {
    try {
        selection(createNodeWithValue(parent, SELECTION), createNodeWithValueAST(parentAST, IF[0]), 1);
    } catch (catchableExpectedToken e) {

        readingBackwardsFlag = 1;
        parent -> children.pop_back();
        parentAST -> children.pop_back();
        try {
            loop(createNodeWithValue(parent, LOOP), createNodeWithValueAST(parentAST, WHILE[0]), 1);
        } catch (catchableExpectedToken e) {

            readingBackwardsFlag = 1;
            parent -> children.pop_back();
            parentAST -> children.pop_back();
            if (tokenProcessingWithoutErrorHandling(parent, ID)) {
                currentId = currentToken[3];
                assignOrFctCallWithoutId(
                    createNodeWithValue(parent, ASSIGNORFCTCALLWITHOUTID), parentAST, 1);
                tokenProcessing(parent, SEMICOLON, 0);
            } else {
                try {
                    node * builtinParent = createNodeWithValueAST(parentAST, FUNCTIONCALL);
                    node * builtinFunctionNameParent = buildASTTerminal(builtinParent, BUILTIN, currentToken);
                    builtInFunctionCall(createNodeWithValue(parent, BUILTINFUNCTIONCALL), builtinParent,
                        1);
                    tokenProcessing(parent, SEMICOLON, 0);
                } catch (catchableExpectedToken e) {
                    readingBackwardsFlag = 1;
                    parent -> children.pop_back();
                    parentAST -> children.pop_back();

                    try {
                        declaration(createNodeWithValue(parent, DECLARATION), createNodeWithValueAST(parentAST, DECLARATION), 1);
                    } catch (catchableExpectedToken e) {

                        readingBackwardsFlag = 1;
                        parent -> children.pop_back();
                        parentAST -> children.pop_back();
                        if (tokenProcessingWithoutErrorHandling(parent, BREAK)) {
                            buildASTTerminal(parentAST, BREAK, currentToken);
                            tokenProcessing(parent, SEMICOLON, 0);
                        } else if (tokenProcessingWithoutErrorHandling(parent, SKIP)) {
                            buildASTTerminal(parentAST, SKIP, currentToken);
                            tokenProcessing(parent, SEMICOLON, 0);
                        } else {
                            try {

                                assignmentForPicker(
                                    createNodeWithValue(parent, ASSIGNMENTFORPICKER), createNodeWithValueAST(parentAST, ASSIGNMENTFORPICKER), 1);
                                tokenProcessing(parent, SEMICOLON, 0);
                            } catch (catchableExpectedToken e) {
                                readingBackwardsFlag = 1;
                                parent -> children.pop_back();
                                parentAST -> children.pop_back();

                                if (tokenProcessingWithoutErrorHandling(parent, RETURN)) {
                                    try {

                                        returned(createNodeWithValue(parent, RETURNED), createNodeWithValueAST(parentAST, RETURN[0]), 1);
                                    } catch (catchableExpectedToken e) {
                                        readingBackwardsFlag = 1;
                                        parent -> children.pop_back();
                                        parentAST -> children.pop_back();
                                        if (!optionality) {
                                            throw expectedToken(errorMessage(
                                                currentToken[0], " a statement ", currentToken[2]));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// This function is used when we have to use builtInFunctionCall subprogram
//<BUILTIN FUNCTION CALL> -> BUILTIN LPAR <PARAMS WITHOUT TYPES>
void builtInFunctionCall(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, BUILTIN, optionality);
    tokenProcessing(parent, LPAR, 0);
    paramsWithoutTypes(createNodeWithValue(parent, PARAMSWITHOUTTYPES), createNodeWithValueAST(parentAST, PARAMS), 0);
}

// This function is used when we have to use numLocators subprogram
//<NUMLOCATORS> -> LSQB NUN RSQB [LSQB NUM RSQB]
void numLocators(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, LSQB, optionality);
    isArray = 1;
    tokenProcessing(parent, NUM, 0);
    buildASTTerminal(parentAST, NUM, currentToken);
    row = stoi(currentToken[3]);
    tokenProcessing(parent, RSQB, 0);
    if (tokenProcessingWithoutErrorHandling(parent, LSQB)) {
        tokenProcessing(parent, NUM, 0);
        buildASTTerminal(parentAST, NUM, currentToken);
        column = stoi(currentToken[3]);
        tokenProcessing(parent, RSQB, 0);
    }
}

// This function is used when we have to use declaration subprogram
//<DECLARATION> -> INT ID [<NUMLOCATORS>]
void declaration(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, INT, optionality);
    tokenProcessing(parent, ID, 0);
    node * idParent = buildASTTerminal(parentAST, ID, currentToken);
    string currentId = currentToken[3];
    // INSERT ALL VARIABLES
    int caught = 0;
    node * locatorsParent = createNodeWithValueAST(idParent, LOCATORS);
    try {
        numLocators(createNodeWithValue(parent, NUMLOCATORS), locatorsParent, 1);
        declareVar(currentId);
        tokenProcessing(parent, SEMICOLON, 0);
    } catch (catchableExpectedToken e) {
        declareVar(currentId);
        caught = 1;
        parent -> children.pop_back();
        idParent -> children.pop_back();
        readingBackwardsFlag = 1;
    }
    if (caught) {
        tokenProcessing(parent, SEMICOLON, 0);
    }
}

// This function is used when we have to use functionCallWithoutId subprogram
//<FUNCTION CALL WITHOUT ID> -> LPAR <PARAMS WITHOUT TYPES>
void functionCallWithoutId(node * parent, node * parentAST, int optionality) {
    tokenProcessing(parent, LPAR, optionality);
    checkFuncIfDefined(currentToken[3]);
    paramsWithoutTypes(createNodeWithValue(parent, PARAMSWITHOUTTYPES), createNodeWithValueAST(parentAST, PARAMS), 0);
    checkParamNumber(currentId);
    nParamsFunctionCall = 0;
}

// This function is used when we have to use paramsWithTypes subprogram
//<Params with types> -> [<otherparams with types]
void paramsWithTypes(node * parent, node * parentAST, int optionality) {
    nParamsFunction = 0;
    try {
        otherParamsWithTypes(createNodeWithValue(parent, OTHERPARAMSWITHTYPES), parentAST, 1);
    } catch (catchableExpectedToken e) {
        parent -> children.pop_back();
        readingBackwardsFlag = 1;
    }
}

// This function is used when we have to use otherParamsWithTypes subprogram
//<other Params with types> -> <types><NUMID>(COMMA<otherparams
// with types>| RPAR)
void otherParamsWithTypes(node * parent, node * parentAST, int optionality) {
    int caught = 0;
    try {
        types(createNodeWithValue(parent, TYPES), parentAST, optionality);
        numId(createNodeWithValue(parent, NUMID), parentAST, 0);
        nParamsFunction += 1;
    } catch (catchableExpectedToken e) {
        caught = 1;
        parent -> children.pop_back();
        readingBackwardsFlag = 1;
    }
    if (tokenProcessingWithoutErrorHandling(parent, RPAR)) {} else {
        tokenProcessing(parent, COMMA, 0);
        otherParamsWithTypes(createNodeWithValue(parent, OTHERPARAMSWITHTYPES), parentAST,
            0);
    }
}

/*
GENERATION OF CODE
FROM THE ABSTRACT TREE TO THE VM LANGUAGE THAT WOULD BE INTERPRETED BY THE FLOWER INTERPRTER
*/

string addMissingZeros(string s) {
    int needed = 3 - s.size();
    for (int i = 0; i < needed; i++) {
        s = "0" + s;
    }
    return s;
}

string addMissingZerosToNumber(string s) {
    int needed = 9 - s.size();
    for (int i = 0; i < needed; i++) {
        s = "0" + s;
    }
    return "+0" + s;
}

string prepareSymbol(string s, string scope) {
    string variableName = s;
    string symbol = addMissingZeros(to_string(symbolsAssigned));
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == variableName && symbolTable[i][4] == scope) {
            symbolTable[i][6] = symbol;
        }
    }
    symbolsAssigned++;
    return symbol;
}

string prepareLocatorsSize(node * root) {
    int x = stoi(root -> children[0] -> value[1]);
    int y = 1;
    if (root -> children.size() > 1)
        y = stoi(root -> children[1] -> value[1]);
    int intSize = x * y;
    string size = addMissingZeros(to_string(intSize));
    return size;
}

void initializeWithInstruction(string instruction) {
    DATASECTION.push_back(instruction);
    DATASECTION.push_back(constant0);
}

void generateDeclaration(node * root, int global) {
    string variableName = root -> children[0] -> value[1];
    string symbol;
    if (global) symbol = prepareSymbol(variableName, GLOBAL);
    else symbol = prepareSymbol(variableName, currentFunctionName);
    string size = "001";
    if (root -> children[0] -> children.size() == 1) {
        node * locators = root -> children[0] -> children[0];
        size = prepareLocatorsSize(locators);
    }
    SPSTART += stoi(size);
    string op = "+3";
    if (global) op = "+0";
    string instruction = op + symbol + size + "000";
    if (global) initializeWithInstruction(instruction);
    else CODESECTION.push_back(instruction);
}

string lookInSmybolToNumericTable(string variableName, string scope) {
    for (int i = 0; i < symbolTable.size(); i++) {
        // cout << symbolTable[i][0] << " " << symbolTable[i][4] << " " <<
        // symbolTable[i][5] << "\n";
        if (symbolTable[i][0] == variableName && (symbolTable[i][4] == scope || symbolTable[i][4] == GLOBAL)) {
            return symbolTable[i][6];
        }
    }
    return "";
}

string operationOpcodeMap(node * root) {
    string opcode = "";
    if (root -> value[1] == "+") {
        opcode = "+1";
    } else if (root -> value[1] == "-") {
        opcode = "-1";
    } else if (root -> value[1] == "*") {
        opcode = "+2";
    } else {
        opcode = "-2";
    }
    return opcode;
}

vector < string > lookForSymbolRow(string variableName, string scope) {
    for (int i = 0; i < symbolTable.size(); i++) {
        if (symbolTable[i][0] == variableName && (symbolTable[i][4] == scope || symbolTable[i][4] == GLOBAL)) {
            return symbolTable[i];
        }
    }
    return {};
}

string findLocatorsPosition(node * root, string variableName) {
    int x = 1;
    int y = 1;
    vector < string > variableSymbolTableRow = lookForSymbolRow(variableName, currentFunctionName);
    int maxY = stoi(variableSymbolTableRow[5]);
    x = stoi(root -> children[0] -> value[1]);
    if (root -> children.size() > 1)
        y = stoi(root -> children[1] -> value[1]);
    string location = addMissingZeros(to_string(x * maxY + y));
    return location;
}

void generateExpr(node * root, int tempN) { // tempN is the offset to decide what temp cache variable we will use to store temp results
    if (root -> value[0] == NUM[0]) {
        string dest = addMissingZeros(to_string((stoi(temp1) + tempN - 1)));
        string instruction = "-0" + dest + "000000";
        CODESECTION.push_back(instruction);
        CODESECTION.push_back(addMissingZerosToNumber(root -> value[1]));
    } else if (root -> value[0] == ID[0]) {
        string location = "000";
        if (root -> children.size() == 1) {
            node * locators = root -> children[0];
            location = findLocatorsPosition(locators, root -> value[1]);
        }
        string numericSymbol = lookInSmybolToNumericTable(root -> value[1], currentFunctionName);
        string dest = addMissingZeros(to_string((stoi(temp1) + tempN - 1)));
        string instruction = "+6" + numericSymbol + location + dest;
        CODESECTION.push_back(instruction);
    } else if (root -> value[0] == PLUSMINUS[0] || root -> value[0] == TIMESDIVIDE[0]) {
        generateExpr(root -> children[0], tempN);
        generateExpr(root -> children[1], tempN + 1);
        string opcode = operationOpcodeMap(root);
        string n1 = addMissingZeros(to_string((stoi(temp1) + tempN - 1)));
        string n2 = addMissingZeros(to_string((stoi(temp1) + tempN)));
        string instruction = opcode + n1 + n2 + n1;
        CODESECTION.push_back(instruction);
    }
}

string lookForFunctionLabel(string functionName) {
    for (int i = 0; i < functionNameToLabel.size(); i++) {
        if (functionNameToLabel[i][0] == functionName) {
            return functionNameToLabel[i][1];
        }
    }
    return "";
}

void generateFunctionCall(node * root, string symbolToAssignTo) {
    node * params = root -> children[1];
    string functionName = root -> children[0] -> value[1];
    if (functionName == "write") {
        node * param = params -> children[0];
        if (param -> value[0] == NUM[0]) {
            CODESECTION.push_back("-0" + temp1 + "000000");
            CODESECTION.push_back(addMissingZerosToNumber(param -> value[1]));
            CODESECTION.push_back("-8" + temp1 + "000000");
        } else {
            CODESECTION.push_back("-8" + lookInSmybolToNumericTable(param -> value[1], currentFunctionName) + "000000");
        }
    } else if (functionName == "read") {
        node * param = params -> children[0];
        // check if it is num too
        CODESECTION.push_back("+8000000" + lookInSmybolToNumericTable(param -> value[1], currentFunctionName));
    } else {
        CODESECTION.push_back("-0" + temp1 + "000000");
        string label = addMissingZeros(to_string(labelsAssigned));
        labelsAssigned++;
        CODESECTION.push_back("+0000000" + addMissingZeros(label));
        string symbol = addMissingZeros(to_string(symbolsAssigned));
        symbolsAssigned++;
        CODESECTION.push_back("+3" + symbol + "001000");
        CODESECTION.push_back("+0" + temp1 + "000" + symbol);
        string newLabel = lookForFunctionLabel(functionName);
        CODESECTION.push_back("+4000000" + newLabel);
        CODESECTION.push_back("-7000000" + label);
        /*
        Very important! Here we decrement the SP because the SP used to point to the cell after the return address because we had to use the return address
        notably in the -3 instruction found in generateFunction. Now that that cell was used and will not be reused, the cell it occupies is compelety free.
        We denode that by moving the SP to the cell where the return address existed.
        */
        CODESECTION.push_back("-1" + SP + "001" + SP);
        if (symbolToAssignTo == "") CODESECTION.push_back("+0" + tempReturn + "000" + temp1);
        else CODESECTION.push_back("+0" + tempReturn + "000" + symbolToAssignTo);
    }
}

void generateAssignment(node * root) {
    string variableName = root -> children[0] -> value[1];
    string numericSymbol = lookInSmybolToNumericTable(variableName, currentFunctionName);
    if (root -> children[1] -> value[0] == FUNCTIONCALL) {
        generateFunctionCall(root -> children[1], numericSymbol);
    } else {
        string size = "000";
        if (root -> children[0] -> children.size() == 1) {
            node * locators = root -> children[0] -> children[0];
            size = findLocatorsPosition(locators, variableName);
        }
        generateExpr(root -> children[1], 1);
        string instruction = "-6" + temp1 + numericSymbol + size;
        CODESECTION.push_back(instruction);
    }
}

void generateStatements(node * cell);

void generateComparison(string op, int temp, string label1, string label2) {
    string dest1 = addMissingZeros(to_string((stoi(temp1) + temp - 1)));
    string dest2 = addMissingZeros(to_string((stoi(temp1) + temp)));
    CODESECTION.push_back(op + dest1 + dest2 + label1); // ???
    CODESECTION.push_back("+0001000" + dest1);
    CODESECTION.push_back("+4000000" + label2);
    CODESECTION.push_back("-7000000" + label1);
    CODESECTION.push_back("+0000000" + dest1);
    CODESECTION.push_back("-7000000" + label2);
}

/*
The second argument (temp) tells in what temp to store the result of the comparison
*/
void generateCondition(node * root, int tempN) {
    string op = root -> value[0];
    if (op == OR[0] || op == AND[0] || op == NOT[0] || op == RELATION[0]) {
        string label1 = addMissingZeros(to_string(labelsAssigned));
        labelsAssigned++;
        string label2 = addMissingZeros(to_string(labelsAssigned));
        labelsAssigned++;
        generateCondition(root -> children[0], tempN);
        generateCondition(root -> children[1], tempN + 1);
        string dest1 = addMissingZeros(to_string((stoi(temp1) + tempN - 1)));
        string dest2 = addMissingZeros(to_string((stoi(temp1) + tempN)));
        if (op == OR[0]) {
            CODESECTION.push_back("+1" + dest1 + dest2 + dest1);
            CODESECTION.push_back("+4" + dest1 + "000" + label1);
            CODESECTION.push_back("+0001000" + dest1);
            CODESECTION.push_back("+4000000" + label2);
            CODESECTION.push_back("-7000000" + label1);
            CODESECTION.push_back("+0000000" + dest1);
            CODESECTION.push_back("-7000000" + label2);
        } else if (op == AND[0]) {
            // both temp1 and temp2 contain temp1 * temp2
            CODESECTION.push_back("+2" + dest1 + dest2 + dest1);
            CODESECTION.push_back("+4" + dest1 + "000" + label1);
            CODESECTION.push_back("+0001000" + dest1);
            CODESECTION.push_back("+4000000" + label2);
            CODESECTION.push_back("-7000000" + label1);
            CODESECTION.push_back("+0000000" + dest1);
            CODESECTION.push_back("-7000000" + label2);
        } else if (op == RELATION[0]) {
            string relationOp = root -> value[1];
            if (relationOp == "<") {
                generateComparison("+5", tempN, label1, label2);
            } else if (relationOp == ">=") {
                generateComparison("-5", tempN, label1, label2);
            } else if (relationOp == ">") {
                generateComparison("+5", tempN, label1, label2);
            } else if (relationOp == "<=") {
                generateComparison("-5", tempN, label1, label2);
            } else if (relationOp == "==") {
                generateComparison("-4", tempN, label1, label2);
            } else if (relationOp == "!=") {
                generateComparison("+4", tempN, label1, label2);
            }
        }
    } else {
        generateExpr(root, tempN);
    }
}

string generateElif(node * root, string ultimateJump) {
    string labelToJumpTo = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    generateCondition(root -> children[0], 1);
    CODESECTION.push_back("+4" + temp1 + "000" + labelToJumpTo);
    generateStatements(root -> children[1]);
    CODESECTION.push_back("+4000000" + ultimateJump);
    return labelToJumpTo;
}

void generateElse(node * root) {
    generateStatements(root -> children[0]);
}

void generateSelection(node * root) {
    string ultimateJump = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    string labelToJumpTo = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    // CODESECTION.push_back("here");
    generateCondition(root -> children[0], 1);
    if (root -> children.size() > 2) CODESECTION.push_back("+4" + temp1 + "000" + labelToJumpTo);
    else CODESECTION.push_back("+4" + temp1 + "000" + ultimateJump);
    // CODESECTION.push_back("here");
    generateStatements(root -> children[1]);
    CODESECTION.push_back("+4000000" + ultimateJump);
    for (int i = 2; i < root -> children.size(); i++) {
        CODESECTION.push_back("-7000000" + labelToJumpTo);
        node * child = root -> children[i];
        if (child -> value[0] == ELIF[0]) {
            labelToJumpTo = generateElif(child, ultimateJump);
        } else if (child -> value[0] == ELSE[0]) {
            generateElse(child);
        }
    }
    CODESECTION.push_back("-7000000" + ultimateJump);
}

void generateLoop(node * cell) {
    string labelToJumpTo = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    string trivialLabel = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    CODESECTION.push_back("-7000000" + trivialLabel);
    generateCondition(cell -> children[0], 1);
    CODESECTION.push_back("+4" + temp1 + "000" + labelToJumpTo);
    generateStatements(cell -> children[1]);
    CODESECTION.push_back("+4000000" + trivialLabel);
    CODESECTION.push_back("-7000000" + labelToJumpTo);
}

void generateReturn(node * root) {
    string returned = root -> children[0] -> value[1];
    if (root -> children[0] -> value[0] == ID[0]) {
        string location = "000";
        if (root -> children[0] -> children.size() == 1) location = findLocatorsPosition(root -> children[0] -> children[0], returned);
        CODESECTION.push_back("+6" + lookInSmybolToNumericTable(returned, currentFunctionName) + location + tempReturn);
    } else {
        CODESECTION.push_back("-0" + tempReturn + "000000");
        CODESECTION.push_back(addMissingZerosToNumber(returned));
    }
    CODESECTION.push_back("+0" + EP + "000" + SP);
    CODESECTION.push_back("-3000000000");
}

void generateStatements(node * root) {
    for (node * child: root -> children) {
        if (child -> value[0] == DECLARATION) {
            generateDeclaration(child, 0); // 0 stands for not global (all variables defined inside a function do not have a global scope)
        }
        if (child -> value[0] == ASSIGNMENT[0]) {
            generateAssignment(child);
        }
        if (child -> value[0] == IF[0]) {
            generateSelection(child);
        }
        if (child -> value[0] == WHILE[0]) {
            generateLoop(child);
        }
        if (child -> value[0] == RETURN[0]) {
            generateReturn(child);
        }
        if (child -> value[0] == FUNCTIONCALL) {
            generateFunctionCall(child, "");
        }
    }
}

void generateFunction(node * root) {
    string functionName = root -> children[1] -> value[1];
    currentFunctionName = functionName;

    // assigning a label to the function and adding the corresponding instruction to the code section
    string label = addMissingZeros(to_string(labelsAssigned));
    labelsAssigned++;
    functionNameToLabel.push_back({
        functionName,
        label
    });
    string instruction = "-7000000" + label;
    CODESECTION.push_back(instruction);

    instruction = "+0" + SP + "000" + EP; // updating the EP tp SP
    CODESECTION.push_back(instruction);
    instruction = "+1001" + SP + SP; // ADD 1 to SP to point to the last free cell
    CODESECTION.push_back(instruction);
    generateStatements(root -> children[3]);
    // SP = EP -> Once we are done from the execution of the unction we change SP to the begining of the activation record
    // CODESECTION.push_back("+0" + EP + "000" + SP);
    // jumping to the return address that SP is now pointing to
    // CODESECTION.push_back("-3000000000");
}

void generateStartFunction(node * root) {
    currentFunctionName = "START";
    CODESECTION.push_back("-7000000000");
    CODESECTION.push_back("-0" + temp1 + "001000");
    CODESECTION.push_back(addMissingZerosToNumber(to_string(SPSTART)));
    CODESECTION.push_back("+0" + temp1 + "000" + SP);
    generateStatements(root -> children[0]);
}

void generateProgram(node * root) {
    for (node * child: root -> children) {
        if (child -> value[0] == DECLARATION) {
            generateDeclaration(child, 1); // 1 stands for global
        } else if (child -> value[0] == FUNCTION) {
            generateFunction(child);
        } else if (child -> value[0] == STARTFUNCTION) {
            generateStartFunction(child);
        }
    }
}

void generateCode(node * root, int level) {
    string symbol = prepareSymbol("WORLD", GLOBAL);
    int intSize = stoi(root -> children[0] -> children[0] -> children[0] -> value[1]) *
        stoi(root -> children[0] -> children[0] -> children[1] -> value[1]);
    string size = addMissingZeros(to_string(intSize));
    string instruction = "+0" + symbol + size + "000";
    SPSTART += stoi(size);
    initializeWithInstruction(instruction);
    CODESECTION.push_back("+4000000000");
    for (node * child: root -> children) {
        if (child -> value[0] == PROGRAM) {
            generateProgram(child);
        }
    }
    CODESECTION.push_back("+9000000000");
    CODESECTION.push_back("+9999999999");
    DATASECTION.push_back("+9999999999");
}

// Main Function
int main() {
    lexerOutput.open("lexerToParser.txt");
    if (!lexerOutput.is_open()) {
        cout << "Could not open file!" << endl;
    }
    try {
        vector < node * > result = code();
        node * headCST = result[0];
        node * headAST = result[1];
        for (int i = 0; i < symbolTable.size(); i++) {
            symbolTable[i].push_back("");
        }
        generateCode(headAST, 0);
        // Important: it prints info the standard io if VERBOSE is 1 and most importantly outputs the files containning the CST, AST, and the generated code
        stdioAndFileOutput(headCST, headAST);
    } catch (expectedToken e) {
        cout << e.what() << "\n";
    } catch (staticSemanticsException e) {
        printSymbolTable();
        cout << e.what() << "\n";
    }
}