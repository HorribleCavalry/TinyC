#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>
#include <vector>
#include <stack>

int token;
char* src, *old_src;

const int poolSize = 1024 * 256;
int line;

int *text,
*old_text,
*stack;
char *data;

int* pc, *bp, *sp, ax, cycle;

enum
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

//int eval()
//{
//	Instruction op = Instruction::NONE;
//	int* temp = nullptr;
//
//	while (true)
//	{
//		op = (Instruction)*pc++;
//		switch (op)
//		{
//		case NONE:
//			break;
//		case LEA: {ax = (int)(bp + *pc++); }
//			break;
//		case IMM: {ax = *pc; ++pc; }
//			break;
//		case JMP: {pc = (int*)*pc; }
//			break;
//		case CALL: {--sp; *sp = (int)(pc + 1); pc = (int*)*pc; }
//			break;
//		case JZ: {pc = ax ? pc + 1 : (int*)*pc; }
//			break;
//		case JNZ: {pc = ax ? (int*)*pc : pc + 1; }
//			break;
//		case ENT: {--sp; *sp = (int)bp; bp = sp; sp = sp - *pc++; }
//			break;
//		case ADJ: {sp = sp + *pc++; }
//			break;
//		case LEV: {sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
//			break;
//		case LI: {ax = *(int*)ax; }
//			break;
//		case LC: {ax = *(char*)ax; }
//			break;
//		case SI: {*(int*)*sp = ax; ++sp; }
//			break;
//		case SC: {*(char*)*sp = ax, ++sp; }
//			break;
//		case PUSH: {--sp; *sp = ax; }
//			break;
//		case OR: {ax = *sp++ | ax; }
//			break;
//		case XOR: {ax = *sp++ ^ ax; }
//			break;
//		case AND: {ax = *sp++ & ax; }
//			break;
//		case EQ: {ax = *sp++ == ax; }
//			break;
//		case NE: {ax = *sp++ != ax; }
//			break;
//		case LT: {ax = *sp++ < ax; }
//			break;
//		case GT: {ax = *sp++ > ax; }
//			break;
//		case LE: {ax = *sp++ <= ax; }
//			break;
//		case GE: {ax = *sp++ >= ax; }
//			break;
//		case SHL: {ax = *sp++ << ax; }
//			break;
//		case SHR: {ax = *sp++ >> ax; }
//			break;
//		case ADD: {ax = *sp++ + ax; }
//			break;
//		case SUB: {ax = *sp++ - ax; }
//			break;
//		case MUL: {ax = *sp++ * ax; }
//			break;
//		case DIV: {ax = *sp++ / ax; }
//			break;
//		case MOD: {ax = *sp++ % ax; }
//			break;
//		case OPEN: 
//			break;
//		case READ:
//			break;
//		case CLOS:
//			break;
//		case PRTF:
//			break;
//		case MALC: {ax = (int)malloc(*sp); }
//			break;
//		case MSET: {ax = (int)memset((char*)sp[2], sp[1], *sp); }
//			break;
//		case MCMP: {ax = memcmp((char *)sp[2], (char *)sp[1], *sp); }
//			break;
//		case EXIT: {printf("Exit(%d)", *sp); return *sp; }
//			break;
//		default: {printf("unknown instruction:%d\n", op); return -1; }
//			break;
//		}
//	}
//}

class TinyCProgram
{
private:
	std::string content;
	std::stack<long long> heapStack;
	unsigned long long line;
	std::vector<long long> memory;

	long long ax;
private:
	void GenerateInstructions()
	{
		memory.push_back(IMM);
		memory.push_back(30);
		memory.push_back(PUSH);
		memory.push_back(IMM);
		memory.push_back(20);
		memory.push_back(SUB);
		memory.push_back(PUSH);
		memory.push_back(EXIT);
	}

public:
	void CompileFile(std::string path)
	{
		std::stringstream buffer;
		std::ifstream inf;
		inf.open(path);
		buffer << inf.rdbuf();
		content = buffer.str();
		inf.close();
		std::cout << "Now compiling C file: " << path << std::endl;
		std::cout << "This content of current file is:" << std::endl;
		std::cout << content << std::endl;

		GenerateInstructions();

		std::cout << "Now compiling finished!" << std::endl;

	}
	void Execute()
	{
		for (int pc = 0; pc < memory.size(); ++pc)
		{
			switch (memory[pc])
			{
			case NONE:
				break;
			case LEA: {/*ax = (int)(bp + *pc++); */}
					  break;
			case IMM: {++pc; ax = memory[pc]; }
					  break;
			case JMP: {pc = memory[pc + 1]; }
					  break;
			case CALL: {/*--sp; *sp = (int)(pc + 1); pc = (int*)*pc; */}
					   break;
			case JZ: {pc = ax ? pc + 1 : memory[pc + 1]; }
					 break;
			case JNZ: {pc = ax ? memory[pc + 1] : pc + 1; }
					  break;
			case ENT: {/*--sp; *sp = (int)bp; bp = sp; sp = sp - *pc++;*/ }
					  break;
			case ADJ: {/*sp = sp + *pc++;*/ }
					  break;
			case LEV: {/*sp = bp; bp = (int *)*sp++; pc = (int *)*sp++;*/ }
					  break;
			case LI: {ax = memory[ax]; }
					 break;
			case LC: {ax = memory[ax]; }
					 break;
			case SI: {/**(int*)*sp = ax; ++sp; */}
					 break;
			case SC: {/**(char*)*sp = ax, ++sp; */}
					 break;
			case PUSH: {heapStack.push(ax);}
					   break;
			case OR: {ax = heapStack.top() | ax; }
					 break;
			case XOR: {ax = heapStack.top() ^ ax; }
					  break;
			case AND: {ax = heapStack.top() & ax; }
					  break;
			case EQ: {ax = heapStack.top() == ax; }
					 break;
			case NE: {ax = heapStack.top() != ax; }
					 break;
			case LT: {ax = heapStack.top() < ax; }
					 break;
			case GT: {ax = heapStack.top() > ax; }
					 break;
			case LE: {ax = heapStack.top() <= ax; }
					 break;
			case GE: {ax = heapStack.top() >= ax; }
					 break;
			case SHL: {ax = heapStack.top() << ax; }
					  break;
			case SHR: {ax = heapStack.top() >> ax; }
					  break;
			case ADD: {ax = heapStack.top() + ax; }
					  break;
			case SUB: {ax = heapStack.top() - ax; }
					  break;
			case MUL: {ax = heapStack.top() * ax; }
					  break;
			case DIV: {ax = heapStack.top() / ax; }
					  break;
			case MOD: {ax = heapStack.top() % ax; }
					  break;
			case OPEN:
				break;
			case READ:
				break;
			case CLOS:
				break;
			case PRTF:
				break;
			case MALC: {/*ax = (int)malloc(*sp); */}
					   break;
			case MSET: {/*ax = (int)memset((char*)sp[2], sp[1], *sp);*/ }
					   break;
			case MCMP: {/*ax = memcmp((char *)sp[2], (char *)sp[1], *sp);*/ }
					   break;
			case EXIT: {std::cout<<heapStack.top()<<std::endl; return; }
					   break;
			default: {std::cout<<"unknown instruction:%d\n"<<heapStack.top(); return; }
					 break;
			}
		}
	}

};

int main(int argc, char** argv)
{
	std::string exePath = argv[0];
	std::string hierarchyPath = exePath.substr(0, exePath.find_last_of('\\') + 1);
	std::string path = hierarchyPath.append("program.c");
	TinyCProgram routine;
	routine.CompileFile(path);

	routine.Execute();

	//line = 1;
	//src = old_src = (char*)malloc(poolSize);
	//text = old_text = (int*)malloc(poolSize);
	//data = (char*)malloc(poolSize);

	//stack = (int*)malloc(poolSize);

	//memset(text, 0, poolSize);
	//memset(data, 0, poolSize);
	//memset(stack, 0, poolSize);

	//bp = sp = (int*)((int)stack + poolSize);
	//ax = 0;

	//int i = 0;
	//text[i++] = IMM;
	//text[i++] = 10;
	//text[i++] = PUSH;
	//text[i++] = IMM;
	//text[i++] = 20;
	//text[i++] = ADD;
	//text[i++] = PUSH;
	//text[i++] = EXIT;

	//pc = text;
	//program();

	//return eval();
}