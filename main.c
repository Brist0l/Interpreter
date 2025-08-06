#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

const char* opcodes_str[] = {
	"PSH", //Push a value
	"ADD", //Add values
	"SUB", //Subtract The Values
	"MUL", //Multiply The Values
	"DIV", //Divide The Values
	"POP", //Remove A value
	"PRT", //Print The Stack
	"HLT", //Terminate the Program
	"SET", //Set values to variables
	"INC", //Increase the value of variable by 1
	"DEC", //Decrease the value of variable by 1
	"INCV", //Increase the value of variable by val
	"DECV", //Decrease the value of variable by val
	"CMP", //Compare a variable with a value
	"LBL", //Define a label 
	"JMP", //Jump to a specific label
	"JNZ", //Jump to a specific label if zero flag is off 
	"JZ" //Jump to a specific label if zero flag is on
};

typedef enum {
	PSH, //Push a value
	ADD, //Add values
	SUB, //Subtract The Values
	MUL, //Multiply The Values
	DIV, //Divide The Values
	POP, //Remove A value
	PRT, //Print The Stack
	HLT,  //Terminate the Program
	SET,//Set values to variables
	INC, //Increase the value of variable by 1
	DEC, //Decrease the value of variable by 1
	INCV, //Increase the value of variable by val
	DECV, //Decrease the value of variable by val
	CMP, //Compare a variable with a value
	LBL, //Define a label 
	JMP,//Jump to a specific label
	JNZ,//Jump to a specific label if zero flag is off 
	JZ //Jump to a specific label if zero flag is on
} opcodes;

int ip = 0;  // instruction Pointer
int sp = 0; // stack Pointer
int zf = 0; // zero flag
int cf = 0; // carry flag
int vr = 0; // var counter
struct ram_hashmap{
	char vars[10];
	int val;
};
struct ram_hashmap ram[10 * sizeof(struct ram_hashmap)]; 
int stack[1024];
bool running = true;
char** program;


void eval(int instr);
int readfile(char *filename, char **readarray,int readarray_size);
int getopcode(char* opcode);
bool checkstacklen(int des_len);
int findvar();

int findvar(){
	for(int i = 0; i < vr; i++)
		if(strcmp(ram[i].vars,program[ip +1]) == 0){
			ip++;
			return i;
		}
	return -1;
}

bool checkstacklen(int des_len){
	if(sp < des_len){
		running = false;
		printf("Stack doesn't have even %d elements\n",des_len);
		return false;
	}
	else
		return true;

}

int getopcode(char* opcode){
	for(int i = 0; i < sizeof(opcodes_str)/sizeof(opcodes_str[0]); i++)
		if(strcmp(opcode, opcodes_str[i]) == 0)
			return i; 
	return -1;  
}

int readfile(char *filename, char **readarray,int readarray_size) {
	FILE *fp;
	fp = fopen(filename, "r");

	if (fp == NULL)
		exit(-1);

	char ch;
	int i = 0;
	int j = 0;
	while((ch = fgetc(fp)) != EOF){
		//Reallocate the program array
		if(i > readarray_size){
			printf("Reallocating some mem cuz the array is smol\n");
			readarray = realloc(readarray,readarray_size);
		}
		if(ch == ' ' || ch == '\n'){
			//printf("%d => %c\n",ch,ch);
			j++;
			i = 0;
			continue;
		}
		//printf("Inserting %c at %dx%d\n",ch,i,j);
		*(*(readarray + j) + i++)= ch;
	}

	fclose(fp);

	return j;
}


void eval(int instr) {
	int a,b,var;

	if(sp >= 1024){
		printf("Stack overflow !\n");
		exit(-1);
	}
	//printf("Instruction => %d\n",instr);
	switch (instr){
		case PSH: 
			var = findvar();
			if(var >= 0)
				stack[sp++] = ram[var].val;
			else
				stack[sp++] = atoi(program[++ip]);

			break;

		case ADD:
			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b + a;
			break;

		case SUB:
			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b - a;
			break;

		case MUL:
			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b * a;
			break;

		case DIV:
			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b / a;
			break;

		case POP: 
			var = findvar();
			if(var >= 0)
				ram[var].val = stack[--sp];
			else
				printf("Popped %d\n", stack[--sp]);
			break;

		case PRT:
			printf("-----stack start----(size of stack = %d)-\n",sp);
			for(int i = sp;i > 0;i--)
				printf("%d\n",stack[i - 1]);
			printf("-----stack end--------------------------\n");
			break;

		case HLT: 
			running = false;
			puts("Stopped!");
			break;

		case SET:
			strcpy(ram[vr].vars, program[++ip]);
			ram[vr++].val = atoi(program[++ip]);
			//			printf("%s has value %d\n",ram[vr -1].vars,ram[vr -1].val);
			break;
		case INC:
			var = findvar();
			if(var >= 0)
				ram[var].val += 1;
			break;

		case DEC:
			var = findvar();
			if(var >= 0)
				ram[var].val -= 1;
			break;

		case INCV:
			var = findvar();
			if(var >= 0)
				ram[var].val += atoi(program[++ip]);
			break;

		case DECV:
			var = findvar();
			if(var >= 0)
				ram[var].val -= atoi(program[++ip]);
			break;

		case CMP:
			for(a = 0; a < vr; a++)
				if(strcmp(ram[a].vars,program[ip +1]) == 0){
					ip++;
					if(ram[a].val == atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is equal to num , zf=1 cf =0\n");
						zf = 1;
						cf = 0;
					}
					else if(ram[a].val > atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is greater than num , zf=0 cf =0\n");
						zf = 0;
						cf = 0;
					}
					else if(ram[a].val < atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is smaller than num , zf=0 cf =1\n");
						zf = 0;
						cf = 1;
					}

					break;
				}
			break;

		case LBL:
			strcpy(ram[vr].vars,program[++ip]); // save the label name in the ram
			ram[vr++].val = ip;
			break;

		case JMP:
			for(a = 0; a < vr; a++)
				if(strcmp(ram[a].vars,program[ip +1]) == 0){
					ip = ram[a].val;
					break;
				}
			break;	
		case JNZ:
			if(zf == 0)
				for(a = 0; a < vr; a++)
					if(strcmp(ram[a].vars,program[ip +1]) == 0){
						ip = ram[a].val;
						break;
					}
			break;	
		case JZ:
			// if zf is turned on 
			if(zf == 1)
				for(a = 0; a < vr; a++)
					if(strcmp(ram[a].vars,program[ip +1]) == 0){
						ip = ram[a].val;
						break;
					}
			break;	
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2){
		fprintf(stderr, "[-] Too Less Or Too Many Args , Provide a valid asm file!");
		exit(-1);
	}

	int size = 1024;
	char **prog = malloc(size * sizeof(char*));   
	for (int i = 0; i < 100; i++)
		prog[i] = malloc(10 * sizeof(char));     

	readfile(argv[1],prog,size);

	program = prog;	

	//printf("instrcution: %s and it's opcode = %d\n",program[ip],getopcode(program[ip]));

	while(running){
		eval(getopcode(program[ip]));
		ip++;
	}

	free(program);
	return 0;
}
