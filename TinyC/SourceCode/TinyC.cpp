#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>

int token;
char* src, *old_src;

const int poolSize = 1024 * 256;
int line;

int *text,
*old_text,
*stack;
char *data;

int* pc, *bp, *sp, ax, cycle;

enum Instruction
{
	NONE,LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
	OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
	OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
};

void next()
{
	token = *src++;
}

void expression(int level)
{

}

void program()
{
	next();
	while (token > 0)
	{
		printf("Token is: %c\n", token);
		next();
	}
}

int eval()
{
	Instruction op = Instruction::NONE;
	int* temp = nullptr;

	while (true)
	{
		op = (Instruction)*pc++;
		switch (op)
		{
		case NONE:
			break;
		case LEA: {ax = (int)(bp + *pc++); }
			break;
		case IMM: {ax = *pc; ++pc; }
			break;
		case JMP: {pc = (int*)*pc; }
			break;
		case CALL: {--sp; *sp = (int)(pc + 1); pc = (int*)*pc; }
			break;
		case JZ: {pc = ax ? pc + 1 : (int*)*pc; }
			break;
		case JNZ: {pc = ax ? (int*)*pc : pc + 1; }
			break;
		case ENT: {--sp; *sp = (int)bp; bp = sp; sp = sp - *pc++; }
			break;
		case ADJ: {sp = sp + *pc++; }
			break;
		case LEV: {sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
			break;
		case LI: {ax = *(int*)ax; }
			break;
		case LC: {ax = *(char*)ax; }
			break;
		case SI: {*(int*)*sp = ax; ++sp; }
			break;
		case SC: {*(char*)*sp = ax, ++sp; }
			break;
		case PUSH: {--sp; *sp = ax; }
			break;
		case OR: {ax = *sp++ | ax; }
			break;
		case XOR: {ax = *sp++ ^ ax; }
			break;
		case AND: {ax = *sp++ & ax; }
			break;
		case EQ: {ax = *sp++ == ax; }
			break;
		case NE: {ax = *sp++ != ax; }
			break;
		case LT: {ax = *sp++ < ax; }
			break;
		case GT: {ax = *sp++ > ax; }
			break;
		case LE: {ax = *sp++ <= ax; }
			break;
		case GE: {ax = *sp++ >= ax; }
			break;
		case SHL: {ax = *sp++ << ax; }
			break;
		case SHR: {ax = *sp++ >> ax; }
			break;
		case ADD: {ax = *sp++ + ax; }
			break;
		case SUB: {ax = *sp++ - ax; }
			break;
		case MUL: {ax = *sp++ * ax; }
			break;
		case DIV: {ax = *sp++ / ax; }
			break;
		case MOD: {ax = *sp++ % ax; }
			break;
		case OPEN: 
			break;
		case READ:
			break;
		case CLOS:
			break;
		case PRTF:
			break;
		case MALC: {ax = (int)malloc(*sp); }
			break;
		case MSET: {ax = (int)memset((char*)sp[2], sp[1], *sp); }
			break;
		case MCMP: {ax = memcmp((char *)sp[2], (char *)sp[1], *sp); }
			break;
		case EXIT: {printf("Exit(%d)", *sp); return *sp; }
			break;
		default: {printf("unknown instruction:%d\n", op); return -1; }
			break;
		}
	}
}

int main(int argc, char** argv)
{
	std::stringstream buffer;
	std::string content;
	std::ifstream inf;
	std::string exePath = argv[0];
	std::string hierarchyPath = exePath.substr(0, exePath.find_last_of('\\') + 1);
	std::string targetPath = hierarchyPath.append("program.c");
	inf.open(targetPath);
	buffer << inf.rdbuf();
	content = buffer.str();
	inf.close();
	std::cout << content<<std::endl;

	line = 1;
	src = old_src = (char*)malloc(poolSize);
	text = old_text = (int*)malloc(poolSize);
	data = (char*)malloc(poolSize);

	stack = (int*)malloc(poolSize);

	memset(text, 0, poolSize);
	memset(data, 0, poolSize);
	memset(stack, 0, poolSize);

	bp = sp = (int*)((int)stack + poolSize);
	ax = 0;

	int i = 0;
	text[i++] = IMM;
	text[i++] = 10;
	text[i++] = PUSH;
	text[i++] = IMM;
	text[i++] = 20;
	text[i++] = ADD;
	text[i++] = PUSH;
	text[i++] = EXIT;

	pc = text;
	program();

	return eval();
}