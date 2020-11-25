#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>

int token;
char* src, *old_src;

const int poolSize = 1024 * 255;
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

void eval()
{
	Instruction op = Instruction::NONE;
	int* temp = nullptr;

	while (true)
	{
		switch (op)
		{
		case NONE:
			break;
		case LEA: {ax = (int)(bp + *pc++); }
			break;
		case IMM: ax = *pc++;
			break;
		case JMP: pc = (int*)*pc;
			break;
		case CALL: {*--sp = (int)(pc + 1); pc = (int*)*pc; }
			break;
		case JZ: pc = ax ? pc + 1 : (int*)*pc;
			break;
		case JNZ: pc = ax ? (int*)*pc : pc + 1;
			break;
		case ENT: {*--sp = (int)bp; bp = sp; sp = sp - *pc++; }
			break;
		case ADJ: {sp = sp + *pc++; }
			break;
		case LEV: {sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
			break;
		case LI: ax = *(char*)ax;
			break;
		case LC:
			break;
		case SI: *(int *)*sp++ = ax;
			break;
		case SC: ax = *(char*)*sp++ = ax;
			break;
		case PUSH: *--sp = ax;
			break;
		case OR:
			break;
		case XOR:
			break;
		case AND:
			break;
		case EQ:
			break;
		case NE:
			break;
		case LT:
			break;
		case GT:
			break;
		case LE:
			break;
		case GE:
			break;
		case SHL:
			break;
		case SHR:
			break;
		case ADD:
			break;
		case SUB:
			break;
		case MUL:
			break;
		case DIV:
			break;
		case MOD:
			break;
		case OPEN:
			break;
		case READ:
			break;
		case CLOS:
			break;
		case PRTF:
			break;
		case MALC:
			break;
		case MSET:
			break;
		case MCMP:
			break;
		case EXIT:
			break;
		default:
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
	std::cout << content;
	inf.close();

	text = old_text = (int*)malloc(poolSize);
	data = (char*)malloc(poolSize);

	stack = (int*)malloc(poolSize);

	memset(text, 0, poolSize);
	memset(data, 0, poolSize);
	memset(stack, 0, poolSize);

	bp = sp = (int*)((int)stack + poolSize);
	ax = 0;

	program();
}