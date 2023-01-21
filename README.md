# The Flower Language
A task-oriented programming language called flower, used to write programs that direct a simple agent, the Picker, to walk through a two-dimensional World, represented as a grid, picking flowers to gather in a bouquet.  The langugae, in its syntay, is very similar to the C/C++ language. Also, The the flower language is a compiled language and and interpreted language (similar to java), and the following different stages of compilation/interpretation, show the different stages and their order.
1. Lexing
2. Parsing
3. Static Semantics
3. Geneating simple virtual machine code
4. Interpretation

## Instructions on running code written in the flower language
To run code written in the flower language, you have to write the code and save it in a file. The extension of the file. does not matter, but in the test folder of this repository, we are giving the files a .fl extension. Then you can run the following command at the root directory:
```
flower.exe path-of-the-file
```
For instance, if I have a file called conditions.fl under the directory test, I can run the code with the following command.
```
flower.exe examples/conditions.fl
```
After running the code, the output will be displayed in console. You will also notice that four files get created, which we will heavily address in the next sections of this README tackling the different stages of compilation/interpretation.

## Design Restrictions
The following stages are, especially the interpretation, are afftected by many design restrictions. The language will run on, presulmably, a non-Von Neumann machine. In other words, the data and the code memory will be separated. Also, the memory has only 2000 cells, divided into 1000 cells for data memory, and the remaining 1000 for code memory. Also, these cells will contain decimal integer values from -9999999999 to 9999999999. As such, many design decisions in the interpretation, will stem from these restrictions.

## Stages of Compilation
### Lexing

### Parsing

### Static Semantics

### Generation

## Interpretation
In the interepretation phase, the input is the VM lanuguage, and the output is the execution of that VM language. However, and before talking about the VM language and how it is designed.
### The VM Language Design
The design of the VM language for the flower language is simple. Each cell of the memory, will contain an instruction or data. In the case of an instruction, we will have 2 types of instructions:
- Labeling instructions
- Computational and flow control instructions
#### Labeling Instructions
There are two types of labeling instructions, and the two types build two tables in the process of interpretation, which are the symbol and label tables. The symbol table stores the mapping between a numerial symbol and a data memory location, Wherease a label label stores the mappiing between a numerical label and a code nmemory location. An easy distinction between labels and symbols, is that symbols refer to memory locations for data, whereas labels refer to memory locations for code.
The instruction for giving a symbol to data is has +0 as an opcode.
The instruction for labeling code has an opcode of -7. 
#### Computational and Flow Control Instructions
These instruction allows the VM language to perform computations on data, such as adding and multiplying, comparing numbers, and jumping from a memory location to another.
### Instructions
The following is the list of instruction supported by the VM for the flower lanuage. An instruction is made of one opcode and 3 operands. The opcode constitutes of one number and the sign, which gives 20 possible opcodes, whereas the operands each have a length of 3 digits.
DML in the following means data memory location.
opcode | arguments | meaning | description
--- | --- | --- | --- |
+0 | src 000 dest | assignment | Assign DM[src] to DML[dst] |
+0 | sym size 000 | creating a new symbol (it is used in the data section only) | creates a new symbol that occupies a specific size in memory and initialize every cell with the number that comes in the subsequent line in the VM language.
-0 | sym 000 000 | assignment | Assign to the DML of the symbol the value in the next cell |
+1 | nn1 nn2 dst | addition | Add DM[nn1] and DM[nn2]; store in DML dst |
-1 | nn1 nn2 dst | subtraction | Add DM[nn1] and DM[nn2]; store in DML dst |
+2 | nn1 nn2 dst | multiplication | multiply DM[nn1] and DM[nn2]; store in DML dst |
-2 | nn1 nn2 dst | division | divide DM[nn1] and DM[nn2]; store in DML dst |
+3 | sym size 000 | creating a new symbol (it is used in the code section of the VML) | creates a new symbol that occupies a specific size in memory and initialize every cell with the number that comes in the subsequent line in the VM language. |
-3 | 000 000 000 | jump | jump to the code memory location stored in the cell before the cell the stack pointer points to |
+4 | nn1 nn2 dst | EQL test | If DM[nn1] = DM[nn2] jump to CML dst |
-4 | nn1 nn2 dst | NEQ test | If DM[nn1] != DM[nn2] jump to CML dst |
+5 | nn1 nn2 dst | GEQ test | Add DM[nn1] and DM[nn2]; store in DML dst |
-5 | nn1 nn2 dst | LT test | If DM[nn1] < DM[nn2] jump to CML dst |
+6 | arr idx dst | read from array | Assign DM[DML[arr] + idx] to DML[dst] |
-6 | src arr idx | LT test | Assign DM[src] to DML[DM[arr] + idx] |
+7 | inc bnd dst | loop | Increment DM[inc]; if DM[inc] < DM[bnd] jump (change IP) to CML dst |
-7 | 000 000 lbl | label | Labels the next line as having name lbl |
+8 | 000 000 dst | read from input | Read from input data and store into DM[dst] |
-8 | src 000 000 | write to output | Write to output DM[src] |
+9 | 000 000 000 | stop | Stops program execution |
-9 | not used | | |