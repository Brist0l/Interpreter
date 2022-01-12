#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  PSH, // Push a value
  ADD, // Add values
  SUB, // Subtract The Values
  MUL, // Multiply The Values
  DIV, // Divide The Values
  POP, // Remove A value
  PRT, // Print The Stack
  HLT  // Terminate the Program
} InstructionSet;

const int program[] = {PSH, 5, PSH, 6, ADD, POP, HLT};

int ip = 0;  // Instruction Pointer
int sp = -1; // Stack Pointer

int stack[256];

bool running = true;

int fetch() { return program[ip]; }

void readFromFile() {
  char ch;

  char file[1024];

  FILE *f;
  f = fopen("./main.asm", "r");
  if (f == NULL) {
    puts("[-]Error Opening File");
    exit(-1);
  }
  
  while(fgets(file,1024,f) != NULL)
	  printf("%s",file);
}


void eval(int instr) {
  switch (instr) {
  case HLT: {
    running = false;
    break;
  }
  case PSH: {
    sp++;
    stack[sp] = program[++ip];
    break;
  }

  case POP: {
    int val_poped = stack[sp--];
    printf("Popped %d\n", val_poped);
    break;
  }

  case ADD: {
    int a = stack[sp--];

    int b = stack[sp--];

    int result = b + a;

    sp++;

    stack[sp] = result;
    break;
  }
  }
}

int main() {
  puts("ok");
  readFromFile();
  printf("tf\n");
  while (running) {
    eval(fetch());
    ip++;
  }
  return 0;
}
