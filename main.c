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

int ip = 0;  // Instruction Pointer
int sp = -1; // Stack Pointer

int stack[256];

bool running = true;

int fetch(char program[]) { return program[ip]; }

int readfile(char *filename, char *readarray) {
  FILE *fp;
  fp = fopen(filename, "r");

  if (fp == NULL) {
    return -1;
  }

  /* fseek goes(seeks) to end of fie and check for error in the if conditional
   */
  if (fseek(fp, 0L, SEEK_END) < 0) {
    fclose(fp);
    return -1;
  }

  int file_size = ftell(fp) + 1;

  /* go back to where we were and continue */
  if (fseek(fp, 0L, SEEK_SET) < 0) {
    printf("Error: Possibly corrupt rom\n");

    return -1;
  }

  /* Read file into array */
  fread(readarray,1024, 1, fp);

  fclose(fp);

  /* return file size so that you know how big is array and prevent bugs in your
   * code */
  readarray[file_size - 1] = '\0';
  return file_size;
}

void eval(int instr) {
  switch (instr) {
  case HLT: {
    running = false;
    puts("Stopped!");
    break;
  }
  case PSH: {
    sp++;
 //   stack[sp] = program[++ip];
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
  char ok[1024];
  readfile("./main.asm", ok);
  
  while (running) {
    eval(fetch(ok));
    ip++;
  }
  return 0;
}
