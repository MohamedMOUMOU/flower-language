#include "headers/output.hpp"
#define VERBOSE 0

ofstream output;

// Function defined to generate spaces while printing the trees
string spaces(int level) {
    string s = "";
    while (level != 0) {
        s += " ";
        level--;
    }
    return s;
}

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

void generationOutput() {
    output.open("../generated.txt");
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

void stdoAndFileOutput(node * headCST, node * headAST) {
    output.open("../concreteSyntaxTree.txt");
    if (VERBOSE) cout << "CONCRETE SYNTAX TREE\n";
    printTree(headCST, 0);
    output.close();
    output.open("../abstractSyntaxTree.txt");
    if (VERBOSE) cout << "CONCRETE SYNTAX TREE\n";
    printTree(headAST, 1);
    output.close();
    if (VERBOSE) printSymbolTable();
    generationOutput();
}