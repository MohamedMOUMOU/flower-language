#include "headers/exceptions.hpp"

// Defining the expected token exception tha should be thrown when the parser
// gets a different token than expected
expectedToken::expectedToken(string msg): message(msg) {};
string expectedToken::what() {
  return message;
}

// Defining the catchableExpectedToken Exception which is thrown whenever we try
// a token and soesn't work mainly when we have an "or" or an optional token
catchableExpectedToken::catchableExpectedToken(string msg): message(msg) {};
string catchableExpectedToken::what() {
  return message;
}

// Defining an exception for static semantics errors
staticSemanticsException::staticSemanticsException(string msg): message(msg) {};
string staticSemanticsException::what() {
  return message;
}

char * InvalidLabel::what() {
  char
  const * str = "Exception: Label not found!";
  return (char * ) str;
}

char * InvalidSymbol::what() {
  char
  const * str = "Exception: Symbol not found!";
  return (char * ) str;
}

// This function prints the information where an error occured. It prints the
// line number, the expected token and the token we got instead.
string errorMessage(string s1, string s2, string s3) {
  return "Line: " + s1 + " - Expected " + s2 + ", got " + s3 + "\n";
}

// This function prints the information where an error occured. It prints the
// line number, the expected token and the token we got instead.
string staticSemanticsErrorMessage(string s) {
  return "Error: " + s;
}