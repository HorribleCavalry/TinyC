#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>
#include <vector>
#include <stack>
#include <utility>


class TinyCProgram
{
public:
	enum
	{
		NONE, LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
		OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
		OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
	};
private:
	std::string content;
	unsigned long long line;
	unsigned long long stackSize;
	unsigned long long memorySize;

	long long* stackPointer;
	long long* stackMemory;
	long long* programCounter;
	long long* programMemory;
	long long ax;

public:
	TinyCProgram()
		:line(1), stackSize(1024*1024), memorySize(1024 * 1024)
	{
		stackPointer = stackMemory = (long long*)malloc(stackSize);
		programCounter = programMemory = (long long*)malloc(memorySize);
	}

	TinyCProgram(unsigned long long _stackSize, unsigned long long _memorySize)
		:line(1), stackSize(_stackSize), memorySize(_memorySize)
	{
		stackPointer = stackMemory = (long long*)malloc(stackSize);
		programCounter = programMemory = (long long*)malloc(memorySize);
	}

private:
	void GenerateInstructions()
	{
		int iterMem = 0;
		programMemory[iterMem++] = IMM;
		programMemory[iterMem++] = 12;
		programMemory[iterMem++] = PUSH;
		programMemory[iterMem++] = IMM;
		programMemory[iterMem++] = 24;
		programMemory[iterMem++] = ADD;
		programMemory[iterMem++] = PUSH;
		programMemory[iterMem++] = EXIT;
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
		long long OP;
		while (programCounter < programMemory + memorySize/sizeof(long long))
		{
			OP = *programCounter;
			switch (OP)
			{
			case NONE:
				break;
			case LEA: {/*ax = (int)(bp + *pc++); */}
					  break;
			case IMM: {++programCounter; ax = *programCounter; }
					  break;
			case JMP: {programCounter = (long long*)*(programCounter + 1); }
					  break;
			case CALL: {/*--sp; *sp = (int)(pc + 1); pc = (int*)*pc; */}
					   break;
			case JZ: {programCounter = ax ? programCounter + 1 : (long long*)*(programCounter + 1); }
					 break;
			case JNZ: {programCounter = ax ? (long long*)*(programCounter + 1) : programCounter + 1; }
					  break;
			case ENT: {/*--sp; *sp = (int)bp; bp = sp; sp = sp - *pc++;*/ }
					  break;
			case ADJ: {/*sp = sp + *pc++;*/ }
					  break;
			case LEV: {/*sp = bp; bp = (int *)*sp++; pc = (int *)*sp++;*/ }
					  break;
			case LI: {ax = programMemory[ax]; }
					 break;
			case LC: {ax = programMemory[ax]; }
					 break;
			case SI: {/**(int*)*sp = ax; ++sp; */}
					 break;
			case SC: {/**(char*)*sp = ax, ++sp; */}
					 break;
			case PUSH: {*stackPointer = ax; ++stackPointer; }
					   break;
			case OR: {ax = *(stackPointer-1) | ax; }
					 break;
			case XOR: {ax = *(stackPointer - 1) ^ ax; }
					  break;
			case AND: {ax = *(stackPointer - 1) & ax; }
					  break;
			case EQ: {ax = *(stackPointer - 1) == ax; }
					 break;
			case NE: {ax = *(stackPointer - 1) != ax; }
					 break;
			case LT: {ax = *(stackPointer - 1) < ax; }
					 break;
			case GT: {ax = *(stackPointer - 1) > ax; }
					 break;
			case LE: {ax = *(stackPointer - 1) <= ax; }
					 break;
			case GE: {ax = *(stackPointer - 1) >= ax; }
					 break;
			case SHL: {ax = *(stackPointer - 1) << ax; }
					  break;
			case SHR: {ax = *(stackPointer - 1) >> ax; }
					  break;
			case ADD: {ax = *(stackPointer - 1) + ax; }
					  break;
			case SUB: {ax = *(stackPointer - 1) - ax; }
					  break;
			case MUL: {ax = *(stackPointer - 1) * ax; }
					  break;
			case DIV: {ax = *(stackPointer - 1) / ax; }
					  break;
			case MOD: {ax = *(stackPointer - 1) % ax; }
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
			case EXIT: {std::cout << *(stackPointer - 1) << std::endl; return; }
					   break;
			default: {std::cout << "unknown instruction:%d\n" << *(stackPointer - 1); return; }
					 break;
			}

			++programCounter;

		}
	}
	~TinyCProgram()
	{
		delete[] stackMemory;
		delete[] programMemory;
	}
};

int main(int argc, char** argv)
{
	std::string exePath = argv[0];
	std::string hierarchyPath = exePath.substr(0, exePath.find_last_of('\\') + 1);
	std::string path = hierarchyPath.append("program.c");
	TinyCProgram routine(8 * sizeof(long long), 8 * sizeof(long long));
	routine.CompileFile(path);

	routine.Execute();

}