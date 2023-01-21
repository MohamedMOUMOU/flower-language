# The Flower Language
A task-oriented programming language called flower used to write programs that direct a simple agent, the Picker, to walk through a two-dimensional World, represented as a grid, picking flowers to gather in a bouquet.  The language, in its syntax, is very similar to the C/C++ language. Also, the flower language is a compiled language and an interpreted language (similar to java), and the following different stages of compilation/interpretation, show the different stages and their order.
1. Lexing
2. Parsing & Static Semantics
3. Generating simple virtual machine code
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
After running the code, the output will be displayed in the console. You will also notice that four files get created, which we will heavily address in the next sections of this README tackling the different stages of compilation/interpretation.

## Design Restrictions
The following stages, especially the interpretation, are affected by many design restrictions. The language will run on, presumably, a non-Von Neumann machine. In other words, the data and the code memory will be separated. Also, the memory has only 2000 cells, divided into 1000 cells for data memory, and the remaining 1000 for code memory. Also, these cells will contain decimal integer values from -9999999999 to 9999999999. As such, many design decisions in the interpretation will stem from these restrictions.

## Stages of Compilation
### Lexing
The lexer works in the following way:
-	First, the lexer takes an input file, which contains the flower code, and then tokenizes (separates lexemes) the file based on white spaces.
-	The second step consists of matching the resulting lexemes to token names that we defined in the lexical description table to return tokens. In many cases, a lexeme would match one token name. However, in cases like the following, “1234;”, a single input would be matched to two token names, int, and semicolon. The lexer will start by reading the input and checking each regular expression defined sequentially, once a regular expression starts matching the input, the lexer would continue with the same regular expression until no match is further available. Then, the lexer would consider the next character a start of a new lexeme and repeats the process.
-	As hinted above, the lexer checks lexemes against regular expressions in the way they were defined (sequentially). Therefore, the ordering of regular expressions is of high importance for the lexer to work properly.

#### Lexical Description table
TOKEN | REGULAR EXPRESSIONS	| EXAMPLE LEXEME | REMARKS
--- | --- | --- | ---
IF | if |	if |	Reserved for selection 
ELSE |	else |	else |	Reserved for selection 
ELIF |	elif |	elif |	Reserved for chaining mutually exclusive conditions 
WHILE |	while |	while |	Reserved for starting loop 
BREAK |	break |	break |	Reserved for loop to go out of the current loop 
SKIP |	skip |	skip |	Reserved for loop to skip the current iteration 
INT |	int |	int |	Reserved for declaring a number, only integers are supported
VOID |	void |	void |	Reserved to signal the return type of a function that does not return anything 
NUM |	0|-?[1-9]{0-10} |	-1,3,444,0 	A number can have up to 10 digits and an optional - sign 
BUILDWORLD |	buildworld |	buildworld |	Reserved to mark the start of building the world. It marks the start of the program file 
WORLDBUILT |	worldbuilt |	worldbuilt |	Reserved to mark the end of building the world. 
PICKER |	PICKER |	picker |	Reserved as an array of two elements marking the coordinates of the picker, and initialized to {0,0} 
FUNC |	func |	func |	Reserved to mark the definition of a user-defined function 
RETURN |	return |	return |	Reserved to return from a function 
LCBR |	{ |	{ |	Delimit the beginning of the scope of a selection, loop, Function definition 
RCBR |	} |	} |	Delimit the end of the scope of a selection, loop, Function definition 
LSQB |	[ |	[ |	Used for locators  
RSQB |	] |	] |	Used for locators 
LPAR |	( |	( |	Used from left for prioritization in arithmetic expressions and to delimit the parameters in a function definition or call, and to delimit the condition of a loop or an arithmetic selection 
RPAR |	) |	) |	Used from the right for prioritization in arithmetic expressions and to delimit the parameters in a function definition or call, and to delimit the condition of a loop or an arithmetic selection 
COMMA |, |, |	Used to separate function parameters in declaration and call 
SEMICOLON |; 	|; |	Used after each statement except loop, selection 
AND |	& |	&| 	Logical binary operator 
OR 	| \|  |  \|	| 	Logical binary operator 
NOT |	! |	! |	Logical unary operator 
RELATION |	==\|<\|>\|<=\|>= \| <!= |	!= 	Relational operators 
ASSIGNMENT |= |	= |	Used for assignment 
PLUSMINUS |	+\|- |	+ |	Arithmetic operators 
TIMESDIVIDE |	*\|/ |	* |	Arithmetic operators 
QUESTIONMARK |	? |	? |	We put it before and after a condition with logical operators to prioritize its execution  
ID |	[a-zA-Z][a-zA-Z0-9]{0, 19} 	| Aya, douae, Mohamed, ayaFlowerCount, douaeNumberMoves, mohamedPosition1, mohamedPosition2  |	User-defined variables that start with a letter, is case sensitive, can contain letters and digits from 0 to 19  
START |	start |	start |	Signals the start of the program after the world is initialized 
BUILTIN |	moveUp\| moveDown\| moveLeft\| moveRight\| senseFlower\| notValidPosition\|senseFence\| pickFlower\| isExit \| isReachable\| read \| write  |	moveUp | 	Used inside program. See explanation below for each built-in function 
EMPTYWORLD 	|makeEmptyWorld |	makeEmptyWorld |	Reserves memory for the world given width and length by the user and initialized it to 0 stands for empty cell 
INITFUNTIONS |	makeFlowerBed \| makeFlower  |	makeFlower |	Used inside World initialization. See explanation below for each built-in function 
#### Input and Output Files of the Lexer
- **Input**: The file containing the flower code.
- **Output**: A file named lexerToPaserl.txt, which contains the tokens of the input code file. This file is passed as the input for the next stage.

### Parsing & Static Semantics
The parser parses the tokens according to the grammar. The flower language parser was implemented using the recursive descent implementation. Also, the parser for the flower language is an LL(1) parser, hence, the grammar avoids left recursion and passes the pairwise disjointness test. As for the static semantics of the code, they were checked while parsing the grammar.
### Grammar
```
<CODE> -> <WORLD INITIALIZATION><PROGRAM>
<WORLD INITIALIZATION> -> BUILDWORLD EMPTYWORLD LPAR NUM COMMA NUM RPAR <INITIALIZATION> WORLDBUILT
<INITIALIZATION> -> <INIT FUNCTIONS> <INITIALIZATION> | Ꜫ
<INIT FUNCTIONS> -> INITFUNC LPAR NUM COMMA NUM [COMMA NUM COMMA NUM] RPAR
<PROGRAM> -> {<FUNCTION>|<DECLARATION> SEMICOLON} <START FUNCTION>
<START FUNCTION> -> START LPAR RPAR LCBR <STATEMENTS>RCBR
<FUNCTION> -> FUNC <TYPES> ID LPAR <PARAMS WITH TYPES> LCBR
<STATEMENTS> RCBR
<TYPES>-> VOID | INT
<RETURNED> -> < NUMIDFUNC> SEMICOLON| Ꜫ
<INT PARAMS> -> [<OTHER INT PARAMS >] <INT ENDPARAM>
<OTHER INT PARAMS> -> INT ID COMMA <OTHER INT PARAMS > | Ꜫ
<INT ENDPARAM > -> INT ID | Ꜫ
<PARAMS WITHOUT TYPES> -> [<OTHER PARAMS WITHOUT TYPES>]
<OTHER PARAMS WITHOUT TYPES> -> (< NUMIDFUNC> | <PICKER>) (COMMA
<OTHER PARAMS WITHOUT TYPES> | RPAR)
<STATEMENTS> -> {<STATEMENT>}
<STATEMENT> -> <SELECTION> | <LOOP> | ID<ASSIGNORFCTCALLWITHOUTID>SEMICOLON | <BUILTINFUNCTIONCALL> SEMICOLON | <DECLARATION> | BREAK SEMICOLON | SKIP SEMICOLON |<ASSIGNMENTFORPICKER> SEMICOLON | RETURN <RETURNED>
<ASSIGNORFCTCALLWITHOUTID> -><FUNCTIONCALLWITHOUTID>|<ASSIGNMENT WITHOUT ID>
<LOOP> -> WHILE LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR
<CONDITIONS> -> <CONDITION1> [AND <CONDITIONS>]
<CONDITION1> -> <CONDITION2> [OR <CONDITION1>]
<CONDITION2> -> [NOT] <CONDITION3>
<CONDITION3> -> <CONDITION> | QUESTIONMARK <CONDITIONS> QUESTIONMARK
<CONDITION> -> <EXPR> [RELATION <EXPR>]
<EXPR> -> <TERM> [PLUSMINUS <EXPR>]
<TERM> -> <FACTOR> [ TIMESDIVIDE <TERM>]
<FACTOR> -> <NUMIDFUNC>| LPAR <EXPR> RPAR
<NUMIDFUNC> -> NUM | ID [<FUNCTION CALL WITHOUT ID> | <LOCATORS>] |
<BUILTINFUNCTION CALL>
<FUNCTION CALL WITHOUT ID> -> LPAR <PARAMS WITHOUT TYPES>
<SELECTION> -> IF LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR [<ELIFS>] [<ELSESTATEMENT>]
<ELIFS> -> ELIF LPAR <CONDITIONS> RPAR LCBR <STATEMENTS> RCBR <ELIFS>| Ꜫ
<ELSESTATEMENT> -> ELSE LCBR <STATEMENTS> RCBR
<ASSIGNMENT WITHOUT ID> -> [<LOCATORS>] ASSIGNMENT <EXPR>
<ASSIGNMENTFORPICKER> -> <PICKER> ASSIGNMENT (<NUMIDFUNC> | <PICKER>)
<LOCATORS> ->LSQB <NUMID> RSQB [ LSQB <NUMID> RSQB]
<NUMID> -> NUM | ID
<DECLARATION> -> INT ID [<NUMLOCATORS>]
<NUMLOCATORS> -> LSQB NUN RSQB [LSQB NUM RSQB]
<BUILTIN FUNCTION CALL> -> BUILTIN LPAR <PARAMS WITHOUT TYPES>
<PICKERWITHLOCATORS>->PICKER LSQB NUM RSQB
<PARAMS WITH TYPES> -> [<OTHER PARAMS WITH TYPES]
<OTHER PARAMS WITH TYPES> -> <TYPES><NUMID> (COMMA<OTHERPARAMSWITH TYPES>| RPAR)
```

#### Static Semantics
If the code is parsed with no errors, it means that the code is syntactically correct. Nonetheless, the latter does not mean that it will not run with no problems. In fact, there might still be some mistakes in the code related to static semantics. In the flower language, we handled some of the static semantics issues, such as:

- Checking whether a variable is declared before it is assigned.
- Checking whether a variable is defined before it is used.
- Checking whether a function is defined before it is called.
- Checking the number of parameters when calling a function.

However, if the code passes the static semantics checks, it is no guarantee that the code will run with no errors, as runtime errors might arise, let alone problems with the semantics of the code.

#### Input and Output Files of the Parser + Static Semantics
- **Input**: The input file used by the parser is lexerToParser.txt which contains the stream of tokens of the flower code submitted along with the line where the token was detected, the token ID, and the value.
- **Output**: two output files: concreteSyntaxTree.txt and abstract SyntaxTree.txt. The first file shows the concrete syntax tree of the flower code submitted, and the second file shows the abstract syntax tree.

### Generation
The generation part is the last stage to arrive at the VM language. The input for this stage is the abstract tree produced in the parsing stage, and also, the symbol table that we built throughout the parsing. Note, in the implementation, the symbol table in the parsing and generation phase is the same. However, in the interpretation phase, we build a new symbol table only containing the information needed. However, the same table could have been kept. The output of this stage is the generated.txt file, which contains the VM language that will be interpreted in the next and last stage. The generation for each construct in the language, such as declaration, assignment, function call, etc., is a little bit complicated and can be investigated in the code. However, the following section will talk about how functions and function calls are designed in our VM language (You might want to read the design of the VML before).
#### Functions and Functions' Call Design
The memory is composed of 2000 words into two parts: the data memory (1000 words) and the code memory (1000 words). In the data memory, we decided to reserve the first 10 cells for some special variables and constants: The first cell for constant 0, the second cell for constant 1, the third for the stack pointer (SP), the fourth for the environment pointer (EP), and the remaining 5 cells for temp variables, and the last two for the picker location (x and y).

The data memory is very flexible because it allows us to allocate memory for all our variables. As we execute a code, we store our global variables in the global and static memory, then a stack gets created to store the activation records for different functions where the variables which are visible in that function are stored. Of course, the stack pointer (SP) and the environment pointer (EP) move accordingly. With SP pointing to the first free memory cell and the last used memory cell, and EP pointing to the first cell of the activation record. Whenever we have a function call,(EP) becomes (SP), and (SP) moves dynamically during runtime

As for the activation records, we decided to keep a simple design, as it will only contain the return address at the beginning of the activation record, followed by the different variables allocated in the function. As our language deals only with two scopes, **global and function scope**, there was no need for static link information at the level of the activation record as we are not implementing static scoping. Nonetheless, we could have added a dynamic link cell in each activation record to support recursion.

## Interpretation
In the interpretation phase, the input is the VM language, and the output is the execution of that VM language. However, before talking about the VM language and how it is designed.
### The VM Language Design
The design of the VM language for the flower language is simple. Each cell of the memory will contain an instruction or data. In the case of instruction, we will have 2 types of instructions:
- Labeling instructions
- Computational and flow control instructions
#### Labeling Instructions
There are two types of labeling instructions, and the two types build two tables in the process of interpretation, which is the symbol and label tables. The symbol table stores the mapping between a numerical symbol and a data memory location, Whereas a label stores the mapping between a numerical label and a code memory location. An easy distinction between labels and symbols is that symbols refer to memory locations for data, whereas labels refer to memory locations for code.
The instruction for giving a symbol to data has +0 as an opcode.
The instruction for labeling code has an opcode of -7. 
#### Computational and Flow Control Instructions
This instruction allows the VM language to perform computations on data, such as adding and multiplying, comparing numbers, and jumping from one memory location to another.
### Instructions
The following is the list of instructions supported by the VM for the flower language. An instruction is made of one opcode and 3 operands. The opcode constitutes one number and the sign, which gives 20 possible opcodes, whereas the operands each have a length of 3 digits.
DML in the following means data memory location.
opcode | arguments | meaning | description
--- | --- | --- | --- |
+0 | src 000 dest | assignment | Assign DM[src] to DML[dst] |
+0 | sym size 000 | creating a new symbol (it is used in the data section only) | creates a new symbol that occupies a specific size in memory and initializes every cell with the number that comes in the subsequent line in the VM language.
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
### Phases of Interpretation
#### Loading
For the loading block, the input is the data and code sections of the program we want to execute, and the output is the data memory populated with data, symbol table completely built, the label table completely built, the program memory populated with instruction, whose symbols and labels are changed to actual memory addresses.
The loading function can be divided into two parts:
- In the data loading part, we assume that all data is symbolized and as such, we read the data section 2 by 2. The first entry corresponds to the symbol, whereas the second one corresponds to the value. In the case of arrays with a size > 1, the symbol is followed by a size > 1, and the next value represents the value by which all the elements of the array are initialized.
- Loading instruction in the program memory. As mentioned before, the symbol and the label table are completely built at this stage. Therefore, we will load into program memory instructions referring to actual memory addresses. 
#### Fetch-Decode-Execute Cycle
For the fetch-decode execute cycle, we perform the following:
- Fetching the instruction: we get the instruction from the code memory.
- Incrementing the instruction pointer.
- Decoding the instruction
- Executing the instruction: The execution for most instructions is done through the function executeInstruction, except for the instruction +8, when we have to read input from the file. In that case, we call another function executeReadInstruction that takes instruction and input as parameters. Also, the instruction “+9000000000” is handled separately by breaking the for-loop that reads and executes the program.
