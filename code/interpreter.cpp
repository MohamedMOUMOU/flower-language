#include "headers/interpreter.hpp"
#include "headers/exceptions.hpp"

// Our data memory has to store 10 digits value thus the long long declaration
long long DATA_MEM[1000];
// Also Instruction are signed 10 digits
long long CODE_MEM[1000];
// Symbol table used for mapping the numeric symbols (keys) to the address (an index of our Data Memory array) where the data corresponding to the symbol is stored
unordered_map<int, int> symbolTable;
// Label table used for mapping the numeric Labels (keys) to the address (an index of Code Memory array) where the labeled instruction is stored
unordered_map<int, int> labelTable;
// Initially the Instruction pointer points to the first cell of CODE_MEM
unordered_map<int, int> literalTable;
vector<int> symbolRunTime;

int IP = 0;
// To avoid name confution and I is added at the SP variable signifying interpreter
const int SP = 002;

// Function to insert values into the data memory and returns the index of the
// last free cell after insertion
int insertIntoMemory(int size, long long value, int lastMemoryCell) {
  int i = lastMemoryCell;
  for (; i < lastMemoryCell + size; i++) {
    DATA_MEM[i] = value;
  }
  return i;
}

// Function to decode an instruction by slicing it according to the layout we
// precised in Instruction struct and returning the instruction
Instruction decodeInstruction(long long instruction) {
  int opcode, operand1, operand2, operand3;
  // we use abs() because only the opcode should hold the sign, the operands
  // should be positive values
  operand1 =
      (int)((std::abs(instruction) / (long long)pow(10, 6)) % (long long)pow(10, 3));
  operand2 =
      (int)((std::abs(instruction) / (long long)pow(10, 3)) % (long long)pow(10, 3));
  operand3 = (int)(std::abs(instruction) % (long long)pow(10, 3));
  opcode = (int)((instruction / (long long)pow(10, 9)) % 10);
  if (instruction < 0 && opcode == 0 ) { 
    opcode = -9;
  }
  
  Instruction ins = {opcode, operand1, operand2, operand3};
  return ins;
}

// This function substitute the symbol by its actual data memory address and the
// label by its actual program memory address, all the necessary arguments are
// passed to it
long long changeOperand(long long instruction, int newOperand,
                       int operandNumber) {
  Instruction ins = decodeInstruction(instruction);
  if (operandNumber == 1)
    ins.operand1 = newOperand;
  if (operandNumber == 2)
    ins.operand2 = newOperand;
  if (operandNumber == 3)
    ins.operand3 = newOperand;
  long long newInstruction = std::abs(ins.opcode) * pow(10, 9) +
                            ins.operand1 * pow(10, 6) +
                            ins.operand2 * pow(10, 3) + ins.operand3;
  if (ins.opcode < 0)
    newInstruction = -newInstruction;
  return newInstruction;
}

// Function calls changeOperand function to substitute only symbols by their
// actual data addresses and returns the new instruction
long long processInstructionForsymbolTable(long long instruction) {
  Instruction ins = decodeInstruction(instruction);
  switch (ins.opcode) {
  // if the instruction is an assignment
  case 0:
    if (symbolTable.find(ins.operand1) == symbolTable.end() ||
        symbolTable.find(ins.operand3) == symbolTable.end())
      throw InvalidSymbol();
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    instruction = changeOperand(instruction, symbolTable[ins.operand3], 3);
    break;
  // if the instruction uses three symbols
  case 1:
  case -1:
  case 2:
  case -2:
  

    if (symbolTable.find(ins.operand1) == symbolTable.end() ||
        symbolTable.find(ins.operand2) == symbolTable.end() ||
        symbolTable.find(ins.operand3) == symbolTable.end()){
          throw InvalidSymbol();  
        }
      
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    instruction = changeOperand(instruction, symbolTable[ins.operand2], 2);
    instruction = changeOperand(instruction, symbolTable[ins.operand3], 3);
    break;
  //instruction ith index as number
  
  case -6:
    if (symbolTable.find(ins.operand1) == symbolTable.end() ||
        symbolTable.find(ins.operand2) == symbolTable.end()){
          throw InvalidSymbol();
        }
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    instruction = changeOperand(instruction, symbolTable[ins.operand2], 2);
    break;
    case 6:
      if (symbolTable.find(ins.operand1) == symbolTable.end() ||
        symbolTable.find(ins.operand3) == symbolTable.end()){
          throw InvalidSymbol();  
        }
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    instruction = changeOperand(instruction, symbolTable[ins.operand3], 3);
    break;
  // if the instruction uses 2 symbols at the beginning
  case 4:
  case -4:
  case 5:
  case -5:
  case 7:
    if (symbolTable.find(ins.operand1) == symbolTable.end() ||
        symbolTable.find(ins.operand2) == symbolTable.end()){
          throw InvalidSymbol();
        }
      
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    instruction = changeOperand(instruction, symbolTable[ins.operand2], 2);
    break;
  // if the instruction is reading an input and storing it in Data cell. It uses
  // one symbol in the end
  case 8:
    if (symbolTable.find(ins.operand3) == symbolTable.end())
      throw InvalidSymbol();
    instruction = changeOperand(instruction, symbolTable[ins.operand3], 3);
    break;
  // if the instruction is writing to the user a value from data memory.It uses
  // one symbol in the beginning
  case -8:
    if (symbolTable.find(ins.operand1) == symbolTable.end())
      throw InvalidSymbol();
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    break;
  case -9:
    if (symbolTable.find(ins.operand1) == symbolTable.end())
      throw InvalidSymbol();
    instruction = changeOperand(instruction, symbolTable[ins.operand1], 1);
    break;
  }
  return instruction;
}

// Function calls changeOperand function to substitute Labels only by their
// actual code address and returns the new instruction to be decoded and
// executed
long long processInstructionForlabelTable(long long instruction) {
  Instruction ins = decodeInstruction(instruction);
  switch (ins.opcode) {
  // if the instruction uses Jump which needs one Label, in 3rd operand
  case 4:
  case -4:
  case 5:
  case -5:
  case 7:
    if (labelTable.find(ins.operand3) == labelTable.end())
      throw InvalidLabel();
    instruction = changeOperand(instruction, labelTable[ins.operand3], 3);
    break;
  }
  return instruction;
}

// Function to execute the instruction
void executeInstruction(Instruction ins) {
  long long sum, difference, product, quotient;
  int stackMemAdd;
  switch (ins.opcode) {

  case 0: // Assign DM[src] to DML dst
    DATA_MEM[ins.operand3] = DATA_MEM[ins.operand1];
    break;

  case 1: // Add DM[nn1] and DM[nn2]; store in DML dst
    sum = DATA_MEM[ins.operand1] + DATA_MEM[ins.operand2];
    DATA_MEM[ins.operand3] =
        (sum > 9999999998 && sum < -9999999999) ? sum : (sum % 9999999999);
    break;

  case -1: // Substract DM[nn2] from DM[nn1]; store in DML dst
    difference = DATA_MEM[ins.operand1] - DATA_MEM[ins.operand2];
    DATA_MEM[ins.operand3] =
        (difference > 9999999998 && difference < -9999999999)
            ? difference
            : (difference % 9999999999);
    break;

  case 2: // Multiply DM[nn1] by DM[nn2]; store in DML dst
    product = DATA_MEM[ins.operand1] * DATA_MEM[ins.operand2];
    DATA_MEM[ins.operand3] = (product > 9999999998 && product < -9999999999)
                                 ? product
                                 : (product % 9999999999);
    break;

  case -2: // Divide DM[nn1] by DM[nn2]; store in DML dst
    quotient = DATA_MEM[ins.operand1] / DATA_MEM[ins.operand2];
    DATA_MEM[ins.operand3] = (quotient > 9999999998 && quotient < -9999999999)
                                 ? quotient
                                 : (quotient % 9999999999);
    break;
  case 3: //+3 symbol size 000
    // create new symbol and map it to address held by SP
    symbolTable[ins.operand1] = DATA_MEM[symbolTable[SP]];
    // increment SP content by the size
    DATA_MEM[symbolTable[SP]] =
        DATA_MEM[symbolTable[SP]] + ins.operand2; //+1 To point to free cell
    break;
  case -3: //-3 000 000 000 jump to the label stored in the memory address SP
           //holds
    // get the memory address of the label
    stackMemAdd = DATA_MEM[DATA_MEM[symbolTable[SP]] - 1]; // Remember that SP points always to the last free cell, as such we have to decrement it to point to the return address
    // get the label inside that memory address and put its corresponding index
    IP = labelTable[stackMemAdd];
    break;
  case 4: // If DM[nn1] = DM[nn2] jump to CML dst
    if (DATA_MEM[ins.operand1] == DATA_MEM[ins.operand2])
      IP = ins.operand3;
    break;

  case -4: // If DM[nn1] != DM[nn2] jump to CML dst
    if (DATA_MEM[ins.operand1] != DATA_MEM[ins.operand2])
      IP = ins.operand3;
    break;

  case 5: // If DM[nn1] >= DM[nn2] jump to CML dst
    if (DATA_MEM[ins.operand1] >= DATA_MEM[ins.operand2])
      IP = ins.operand3;
    break;

  case -5: // If DM[nn1] < DM[nn2] jump to CML dst
    if (DATA_MEM[ins.operand1] < DATA_MEM[ins.operand2])
      IP = ins.operand3;
    break;

  case 6: // Assign DM[ DM[arr]+DM[idx] ] to DML dst
    DATA_MEM[ins.operand3] = DATA_MEM[ins.operand1 + ins.operand2];
    break;

  case -6: // ssign DM[src] to DML DM[arr]+DM[idx]
    DATA_MEM[ins.operand2 + ins.operand3] = DATA_MEM[ins.operand1];
    break;

  case 7: // Increment DM[inc]; if DM[inc] < DM[bnd] jump (change IP) to CML dst
    DATA_MEM[ins.operand1]++;
    if (DATA_MEM[ins.operand1] < DATA_MEM[ins.operand2])
      IP = ins.operand3;
    break;

  case -8: // Write DM[src] to output
    cout << DATA_MEM[ins.operand1] << "\n";
    break;
  case -9:
    DATA_MEM[symbolTable[ins.operand1]] = literalTable[IP - 1];
    break;
    // The other cases are handled seperately, namely +9,+8,-7.
  }
}

// Function to execute +8, store input in a variable in DATA_MEM
void executeReadInstruction(Instruction ins, long long input) {
  DATA_MEM[ins.operand3] = input;
}

int interpretCode() {
    // initiliaze the symbol table to contain constant 0, 1,
  // SP,EP,temp1,temp2,temp3,temp_return,picker row,picker column
  for (int i = 0; i <= 8; i++) {
    symbolTable[i] = i;
  }
  symbolTable[9] = 9;
  DATA_MEM[1]=1;
  long long instructionFromFile;
  // Opening the file
  ifstream input_file("generated.txt");
  if (!input_file.is_open()) {
    cout << "Could not open the file!" << endl;
  }
  // stage 0 => data, stage 1 => program, stage 2 => input
  int stage = 0;
  int lastDataMemoryCell = 11;
  int lastProgramMemoryCell = 0;
  long long TEMP_CODE[1000];
  // Loading data & instructions block
  // Keep reading the program file until EOF reached
  int fileLine = 0;
  long long fileLineInstructionMapping[1000];
  int number;
  int counter = 0;

  while (input_file >> instructionFromFile) {
    ++fileLine;
    long long instruction = instructionFromFile;
    // when we reach +999999999 instruction we increment the stage
    if (instruction == 9999999999) {
      stage++;
      if (stage == 2)
        // we break because at this stage Data and Code are loaded, we will
        // continue reading in execution phase if +8 is encountered
        break;
      // 9999999999 is just a seperator not a instuction nor data, thus, it is
      // not processed
      continue;
    }
    // Decode the instruction and store it laid out into opcode and 3 operands
    // in ins
    Instruction ins = decodeInstruction(instruction);
    if (stage == 0) { // Data
      long long value;
      // read from file the data value which follows the symbol and size
      // declaration
      input_file >> value;
      ++fileLine;
      int symbol = ins.operand1;
      int size = ins.operand2;
      // Build the symbol table. Map the symbol to the address of last free cell
      symbolTable[symbol] = lastDataMemoryCell;
      // Insert the value(s) in DATA_MEM
      lastDataMemoryCell = insertIntoMemory(size, value, lastDataMemoryCell);
    } else if (stage == 1) { // Instruction/program
      // if it is a Label declaration
      if (ins.opcode == -7) {
        long long nextInstruction;
        // read from file the instruction to be labeled
        input_file >> nextInstruction;
        Instruction ins2 = decodeInstruction(nextInstruction);
        if(ins2.opcode==-9){
          nextInstruction=-(std::abs(ins2.opcode) * pow(10, 9) +
                            ins2.operand1 * pow(10, 6) +
                            ins2.operand2 * pow(10, 3) + ins2.operand3);
          input_file >> number;
          literalTable[lastProgramMemoryCell] = number;
          ++fileLine;
        }
        ++fileLine;
        // Build the Label table. Map the numeric Label to the address of last
        // free cell of CODE_MEM
        labelTable[ins.operand3] = lastProgramMemoryCell;
        // nextInstruction is the one that should be loaded, not label
        // declaration thus the assignment
        instruction = nextInstruction;
      } else if (ins.opcode == -9) {
        instruction=-(std::abs(ins.opcode) * pow(10, 9) +
                            ins.operand1 * pow(10, 6) +
                            ins.operand2 * pow(10, 3) + ins.operand3);
        input_file >> number;
        literalTable[lastProgramMemoryCell] = number;
        ++fileLine;
      }
      // We substitute the symbols by actual data addresses
      // We store instructions in CODE_MEM instruction with unprocessed Labels,
      // explanation in report
      TEMP_CODE[lastProgramMemoryCell] = instruction;
      if (ins.opcode == -9 || ins.opcode == -7) {
        fileLineInstructionMapping[lastProgramMemoryCell] = fileLine - 1;
      } else
        fileLineInstructionMapping[lastProgramMemoryCell] = fileLine;
      lastProgramMemoryCell++;
    }
  }
  
  for (int i = 0; i < lastProgramMemoryCell; i++) {
    long long processedInstruction;
    try {
      processedInstruction = processInstructionForlabelTable(TEMP_CODE[i]);
    } catch (InvalidLabel &e) {
      cerr << "File line: " << fileLineInstructionMapping[i] << " -- "
           << e.what() << endl;
      return -1;
    }
    CODE_MEM[i] = processedInstruction;
  }

  // Fetch Decode Execute block
  for (; IP < lastProgramMemoryCell;) {
    // We increment the IP after fetching
    // Fetching
    long long instruction = CODE_MEM[IP];
    IP++;
    instruction=processInstructionForsymbolTable(instruction);
    // Decoding
    
    Instruction ins = decodeInstruction(instruction);
    // Executing
    long long input;
    if (ins.opcode == 8) {
      // Read Input form file
      input_file >> input;
      executeReadInstruction(ins, input);
    } else if (ins.opcode == 9) {
      // Stop program execution
      break;
      // Aya
    } else{
      
      executeInstruction(ins);
    }
  }
  input_file.close();
  return 0;
}