#include "headers/generator.hpp"

using namespace std;

/*
GENERATION OF CODE
FROM THE ABSTRACT TREE TO THE VM LANGUAGE THAT WOULD BE INTERPRETED BY THE FLOWER INTERPRTER
*/

// Declaring generation variables
vector < string > DATASECTION;
vector < string > CODESECTION;
const string SP = "002";
const string EP = "003";
const string temp1 = "004";
const string temp2 = "005";
const string temp3 = "006";
const string temp4 = "007";
const string tempReturn = "008";
const string picker = "009";
const string constant0 = "+0000000000";
int symbolsAssigned = 11;
int labelsAssigned = 1;
int SPSTART = symbolsAssigned;
vector < vector < string >> functionNameToLabel;
string currentFunctionName = "";

// symbolTable built in the time of static semantics
extern vector < vector < string >> symbolTable;

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
    // SP = EP -> Once we are done from the execution of the function we change SP to the begining of the activation record
    CODESECTION.push_back("+0" + EP + "000" + SP);
    // jumping to the return address that SP is now pointing to
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