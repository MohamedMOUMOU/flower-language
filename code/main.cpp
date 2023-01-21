#include "headers/exceptions.hpp"

#include "headers/output.hpp"

#include "headers/lexer.h"

#include "headers/parser-static-semantics.hpp"

#include "headers/generator.hpp"

#include "headers/interpreter.hpp"

int main(int argc, char * argv[]) {
  if (argc > 1) {
    lex(argv[1]);
  } else {
    cout << "No file  path was given!";
    return -1;
  }
  try {
    vector < node * > result = parseAndStaticSemantics();
    node * headCST = result[0];
    node * headAST = result[1];
    for (int i = 0; i < symbolTable.size(); i++) {
      symbolTable[i].push_back("");
    }
    generateCode(headAST, 0);
    // Important: it prints info the standard io if VERBOSE is 1 and most importantly outputs the files containning the CST, AST, and the generated code
    stdOAndFileOutput(headCST, headAST);
  } catch (expectedToken e) {
    cout << e.what() << "\n";
  } catch (staticSemanticsException e) {
    printSymbolTable();
    cout << e.what() << "\n";
  }
  return interpretCode();
}