#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <iostream>

#include <fstream>

#include <vector>

#include <math.h>

#include <string>

#include <unordered_map>

using namespace std;

// This is the layout of an instruction opcode(signed int: sign + digit)
// operand1 & 2& 3 (0-999:3 digits)
typedef struct {
  int opcode;
  int operand1;
  int operand2;
  int operand3;
}
Instruction;

// Interpretation Functions
int interpretCode();

#endif