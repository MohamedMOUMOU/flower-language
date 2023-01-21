#include "headers/parser-static-semantics.hpp"

#include "headers/exceptions.hpp"

// Lexer output
ifstream lexerOutput;
vector < vector < string >> symbolTable;

int readingBackwardsFlag = 0;
vector < string > currentToken = {};
string currentFunctionScope = "GLOBAL";
int nParamsFunction = 0;
int nParamsFunctionCall = 0;
string currentId;
int row = 1;
int column = 1;
int isArray = 0;
int functionDefinitionParam = 0;

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
vector < node * > parseAndStaticSemantics() {
  lexerOutput.open("lexerToParser.txt");
  if (!lexerOutput.is_open()) {
    cout << "Could not open file!" << endl;
  }
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
  lexerOutput.close();
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