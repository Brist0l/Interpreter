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
	"HLT", //Terminate the Progvar
	"SET", //Set values to variables
	"INC", //Increase the value of variable by 1
	"DEC", //Decrease the value of variable by 1
	"INCV", //Increase the value of variable by val
	"DECV", //Decrease the value of variable by val
	"CMP", //Compare a variable with a value
	"LBL", //Define a label 
	"JMP", //Jump to a specific label
	"JNZ", //Jump to a specific label if zero flag is off 
	"JZ", //Jump to a specific label if zero flag is on
	"JG", //Jump to a specific label if var1 > var2
	"JL", //Jump to a specific label if var1 < var2
	"JE", //Jump to a specific label if var1 = var2
	"INP", //Take input into a variable from the user
	"OUT", //Print out the value of the variable
	"CALL",//Call a label
	"RET" // Return
};

typedef enum {
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
} opcodes;

int ip = 0;  // instruction Pointer
int sp = 0; // stack Pointer
int zf = 0; // zero flag
int cf = 0; // carry flag
int vr = 0; // var counter
int lr = 0; // label counter
int fc = 0; // function counter
struct var_hashmap{
	char vars[10];
	int val;
};
struct var_hashmap var[10 * sizeof(struct var_hashmap)]; 
struct label_hashmap{
	char labels[10];
	int inst_index;
};
struct label_hashmap label[10 * sizeof(struct label_hashmap)]; 
int stack[1024];
int call_stack[10];
bool running = true;
char** program;


void eval(int instr);
int readfile(char *filename, char ***readarray_ptr,int readarray_size);
int getopcode(char* opcode);
bool checkstacklen(int des_len);
int findvar();
void gotolabel();
void parselabels(int size);

void parselabels(int size){
	for(int i = 0;i < size;i++)
		if(strcmp(program[i],"LBL") == 0){
			strcpy(label[lr].labels,program[++i]); // save the label name in the ram
			label[lr++].inst_index = i;
		}
}

void gotolabel(){
	for(int i = 0; i < lr; i++)
		if(strcmp(label[i].labels,program[ip +1]) == 0){
			ip = label[i].inst_index;
			break;
		}
}

int findvar(){
	for(int i = 0; i < vr; i++)
		if(strcmp(var[i].vars,program[ip +1]) == 0){
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

int readfile(char *filename, char ***readarray_ptr,int readarray_size) {
	FILE *fp;
	fp = fopen(filename, "r");

	if (fp == NULL)
		exit(-1);

	char ch;
	int i = 0;
	int j = 0;
	char **readarray = *readarray_ptr;
	while((ch = fgetc(fp)) != EOF){
		//Reallocate if j >= current size
		if(j >= readarray_size){
			int old_size = readarray_size;
			readarray_size *= 2; // double the size
					     
			//printf("Reallocating some mem cuz the array is smol, new size: %d\n",readarray_size);

			char **new_arr = realloc(readarray, readarray_size * sizeof(char*));
			for (int r = old_size; r < readarray_size; r++)
				*(new_arr + r) = malloc(10 * sizeof(char));
			readarray = new_arr;
			*readarray_ptr = readarray;
		}
		if(ch == ' ' || ch == '\n'){
			//printf("%d => %c\n",ch,ch);
			j++;
			i = 0;
			continue;
		}
		//printf("Inserting %c at %dx%d\n",ch,i,j);
		*(*(readarray + j) + i++)= ch;
		*(*(readarray + j) + i)= '\0';
	}

	fclose(fp);

	return j;
}

void eval(int instr) {
	int a,b,var_cnt,var_cnt_alt;

	if(sp >= 1024){
		printf("Stack overflow !\n");
		exit(-1);
	}
	//printf("Instruction => %d\n",instr);
	switch (instr){
		case PSH: 
			var_cnt = findvar();
			if(var_cnt >= 0)
				stack[sp++] = var[var_cnt].val;
			else
				stack[sp++] = atoi(program[++ip]);

			break;

		case ADD:
			var_cnt = findvar();
			if(var_cnt >= 0){
				var_cnt_alt = findvar();
				if(var_cnt_alt >= 0){
					var[var_cnt].val += var[var_cnt_alt].val;
					break;
				}
			}

			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b + a;
			break;

		case SUB:
			var_cnt = findvar();
			if(var_cnt >= 0){
				var_cnt_alt = findvar();
				if(var_cnt_alt >= 0){
					var[var_cnt].val -= var[var_cnt_alt].val;
					break;
				}
			}

			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b - a;
			break;

		case MUL:
			var_cnt = findvar();
			if(var_cnt >= 0){
				var_cnt_alt = findvar();
				if(var_cnt_alt >= 0){
					var[var_cnt].val *= var[var_cnt_alt].val;
					break;
				}
			}


			if(!checkstacklen(2))
				break;

			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b * a;
			break;

		case DIV:
			var_cnt = findvar();
			if(var_cnt >= 0){
				var_cnt_alt = findvar();
				if(var_cnt_alt >= 0){
					var[var_cnt].val /= var[var_cnt_alt].val;
					break;
				}
			}

			if(!checkstacklen(2))
				break;
			a = stack[--sp];
			b = stack[--sp];
			stack[sp++] = b / a;
			break;

		case POP: 
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val = stack[--sp];
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
			strcpy(var[vr].vars, program[++ip]);
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[vr++].val = var[var_cnt].val;
			else
				var[vr++].val = atoi(program[++ip]);
			//printf("%s has value %d\n",var[vr -1].vars,ram[vr -1].val);
			break;
		case INC:
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val += 1;
			break;

		case DEC:
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val -= 1;
			break;

		case INCV:
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val += atoi(program[++ip]);
			break;

		case DECV:
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val -= atoi(program[++ip]);
			break;

		case CMP:
			var_cnt = findvar();
			if(var_cnt >= 0){
				var_cnt_alt = findvar();	
				if(var_cnt_alt >= 0){
					//printf("Found another variable\n");
					if(var[var_cnt].val > var[var_cnt_alt].val){
						//printf("Checking %d > %d\n",var[var_cnt].val,var[var_cnt_alt].val);
						//printf("varialbe is greater than num , zf=0 cf =0\n");
						zf = 0;
						cf = 0;
					}
					else if(var[var_cnt].val < var[var_cnt_alt].val){
						//printf("Checking %d < %d\n",var[var_cnt].val,var[var_cnt_alt].val);
						//printf("varialbe is smaller than num , zf=0 cf =1\n");
						zf = 0;
						cf = 1;
					}
					else if(var[var_cnt].val == var[var_cnt_alt].val){
						//printf("Checking %d == %d\n",var[var_cnt].val,var[var_cnt_alt].val);
						//printf("varialbe is equal to num , zf=1 cf =0\n");
						zf = 1;
						cf = 0;
					}

				}
				else{
					if(var[var_cnt].val > atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is greater than num , zf=0 cf =0\n");
						zf = 0;
						cf = 0;
					}
					else if(var[var_cnt].val < atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is smaller than num , zf=0 cf =1\n");
						zf = 0;
						cf = 1;
					}
					else if(var[var_cnt].val == atoi(program[ip + 1])){
						ip++;
						//printf("varialbe is equal to num , zf=1 cf =0\n");
						zf = 1;
						cf = 0;
					}
				}

			}

			break;

		case LBL:
			break;

		case JMP:
			gotolabel();
			break;	

		case JNZ:
			if(zf == 0)
				gotolabel();
			break;	

		case JZ:
			// if zf is turned on 
			if(zf == 1)
				gotolabel();
			break;	

		case JG:
			if(zf == 0 && cf == 0)
				gotolabel();
			break;

		case JL:
			if(zf == 0 && cf == 1)
				gotolabel();
			break;

		case JE:
			if(zf == 1 && cf == 0)
				gotolabel();
			break;
		case INP:
			scanf("%d",&a);
			var_cnt = findvar();
			if(var_cnt >= 0)
				var[var_cnt].val = a;
			else{
				strcpy(var[vr].vars, program[++ip]);
				var[vr++].val = a;
			}

			break;
		case OUT:
			var_cnt = findvar();
			if(var_cnt >= 0)
				printf("%d\n",var[var_cnt].val);

			break;

		case CALL:
			call_stack[fc++] = ip; // instruction to which it needs to come back to
			gotolabel();
			break;

		case RET:
			ip = call_stack[--fc];
			break;

	}
}

int main(int argc, char *argv[]) {
	if (argc != 2){
		fprintf(stderr, "[-] Too Less Or Too Many Args , Provide a valid asm file!");
		exit(-1);
	}

	int size = 100;
	char **prog = malloc(size * sizeof(char*));   
	for (int i = 0; i < size; i++)
		prog[i] = malloc(10 * sizeof(char));     

	size = readfile(argv[1],&prog,size);

	program = prog;	

	//printf("instrcution: %s",program[ip]);
	//printf("and it's opcode = %d\n",getopcode(program[ip]));

	parselabels(size);

	while(running){
		eval(getopcode(program[ip]));
		ip++;
	}

	for(int i = 0; i < size; i++)
		free(program[i]);
	free(program);
	return 0;
}
