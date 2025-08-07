# MiniLang Interpreter

A lightweight interpreter for a custom-designed minimal language. The project focuses on parsing, evaluating, and executing basic arithmetic expressions and simple control structures, designed to resemble a streamlined form of interpreted assembly.


## Usage

Clone the repository:

```
git clone https://github.com/Brist0l/Interpreter
cd Interpreter
```

Compile and run:

`make`

Run a script:

    ./interpret sample_scripts/factorial.asm

## Language Specification (WIP)

### Data Types

1) Integers
2) (Future support for booleans, strings, arrays)

### Operators

1) Arithmetic: +, -, *, /
2) Assignment: =
3) Comparison: ==, !=, <, >, <=, >= (WIP)

### Control Flow

1) if, else
2) Function definitions and calls (Planned)

## Current Instruction Set

```
PSH, //Push a value
ADD, //Add values
SUB, //Subtract The Values
MUL, //Multiply The Values
DIV, //Divide The Values
POP, //Remove A value
PRT, //Print The Stack
HLT,  //Terminate the Progvar
SET,//Set values to variables
INC, //Increase the value of variable by 1
DEC, //Decrease the value of variable by 1
INCV, //Increase the value of variable by val
DECV, //Decrease the value of variable by val
CMP, //Compare a variable with a value
LBL, //Define a label
JMP,//Jump to a specific label
JNZ,//Jump to a specific label if zero flag is off
JZ ,//Jump to a specific label if zero flag is on
JG, //Jump to a specific label if var1 > var2
JL, //Jump to a specific label if var1 < var2
JE, //Jump to a specific label if var1 = var2
INP, //Take input into a variable from the user
OUT, //Print out the value of the variable
CALL, //Call a label
RET // Return
```

### Example Program
```asm
PSH 10
CALL func
PSH 99
PRT
HLT

LBL func
PSH 20
ADD
RET
```

This program pushes 10 to the stack then goes to the `func` function and in there it pushes 20 to 
the stack, 20 and 10 are popped off the stack and added and then this returns back to where it 
branched off from. It pushes 99 on the stack and then prints the stack and exits.

![output](https://github.com/Brist0l/Interpreter/blob/main/imgs/output.png)

