﻿#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>
#include <vector>
#include <stack>
#include <utility>
#include <unordered_map>
#include <map>


class TinyCProgram
{
public:
	enum
	{
		NONE, LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
		OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
		OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
	};

	enum { Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize };

	struct StructDefinition
	{
		std::string structName;
		unsigned long long byteSize = 0;

		std::map<std::string, StructDefinition> membersMap;
	};

	struct FunctionDefinition
	{
		unsigned long long paramNum = 0;
		std::string returnTypeName;
		std::string functionName;
		std::map<std::string, StructDefinition> localVariable;
		std::vector<FunctionDefinition> instructions;
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

	char token;
	char* src;
	char* old_src;

	int* current_id;
	int* symbols;

	std::map<std::string, StructDefinition> structDefMap;
	std::map<std::string, StructDefinition> variableMap;
	std::map<std::string, FunctionDefinition> functionMap;

public:
	TinyCProgram()
		:line(1), stackSize(1024*1024), memorySize(1024 * 1024)
	{
		stackPointer = stackMemory = (long long*)malloc(stackSize);
		programCounter = programMemory = (long long*)malloc(memorySize);
		initBasicStruct();
	}

	TinyCProgram(unsigned long long _stackSize, unsigned long long _memorySize)
		:line(1), stackSize(_stackSize), memorySize(_memorySize)
	{
		stackPointer = stackMemory = (long long*)malloc(stackSize);
		programCounter = programMemory = (long long*)malloc(memorySize);
		initBasicStruct();
	}

private:
	unsigned long long charIdx = 0;

	void initBasicStruct()
	{
		StructDefinition boolDef;
		StructDefinition charDef;
		StructDefinition unsignedCharDef;
		StructDefinition shortDef;
		StructDefinition unsignedShortDef;
		StructDefinition intDef;
		StructDefinition unsignedIntDef;
		StructDefinition longDef;
		StructDefinition unsignedLongDef;
		StructDefinition longLongDef;
		StructDefinition unsignedLongLongDef;
		StructDefinition floatDef;
		StructDefinition doubleDef;
		StructDefinition pointerDef;

		boolDef.byteSize = sizeof(bool);
		boolDef.structName = "bool";

		charDef.byteSize = sizeof(char);
		charDef.structName = "char";

		unsignedCharDef.byteSize = sizeof(unsigned char);
		unsignedCharDef.structName = "unsigned char";

		shortDef.byteSize = sizeof(short);
		shortDef.structName = "short";

		unsignedShortDef.byteSize = sizeof(unsigned short);
		unsignedShortDef.structName = "unsigned short";

		intDef.byteSize = sizeof(int);
		intDef.structName = "int";

		unsignedIntDef.byteSize = sizeof(unsigned int);
		unsignedIntDef.structName = "unsigned int";

		longDef.byteSize = sizeof(long);
		longDef.structName = "long";

		unsignedLongDef.byteSize = sizeof(unsigned long);
		unsignedLongDef.structName = "unsigned long";

		longLongDef.byteSize = sizeof(long long);
		longLongDef.structName = "long long";

		unsignedLongLongDef.byteSize = sizeof(unsigned long long);
		unsignedLongLongDef.structName = "unsigned long long";

		floatDef.byteSize = sizeof(float);
		floatDef.structName = "float";

		doubleDef.byteSize = sizeof(double);
		doubleDef.structName = "double";

		pointerDef.byteSize = sizeof(void*);
		pointerDef.structName = "void*";

		structDefMap["bool"] = boolDef;
		structDefMap["char"] = charDef;
		structDefMap["unsigned char"] = unsignedCharDef;
		structDefMap["short"] = shortDef;
		structDefMap["unsigned short"] = unsignedShortDef;
		structDefMap["int"] = intDef;
		structDefMap["unsigned int"] = unsignedIntDef;
		structDefMap["long"] = longDef;
		structDefMap["unsigned long"] = unsignedLongDef;
		structDefMap["long long"] = longLongDef;
		structDefMap["unsigned long long"] = unsignedLongLongDef;
		structDefMap["float"] = floatDef;
		structDefMap["double"] = doubleDef;

		for (auto it : structDefMap)
		{
			std::cout << "The type is " << it.second.structName << ", and its byteSize is " << it.second.byteSize << std::endl;
		}
	}

	struct WordAndIdx
	{
		unsigned long long startIdx;
		unsigned long long endIdx;
		unsigned long long line;
		std::string word;
		WordAndIdx(unsigned long long _startIdx, unsigned long long _endIdx, unsigned long long _line)
			:startIdx(_startIdx), endIdx(_endIdx), line(_line) {}
	};

	const WordAndIdx GetNextWord(unsigned long long startIdx, unsigned long long line)
	{
		WordAndIdx result(startIdx, startIdx, line);
		while (true)
		{
			token = content[result.endIdx];
			if (token == '\n')
			{
				++result.line;
			}
			else if (token == '#')
			{
				while (content[result.endIdx] != '\n')
				{
					++result.endIdx;
				}
				++result.line;
			}
			else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
			{
				result.startIdx = result.endIdx;

				result.word.erase(result.word.begin(), result.word.end());
				while ((content[result.endIdx] >= 'a' && content[result.endIdx] <= 'z') || (content[result.endIdx] >= 'A' && content[result.endIdx] <= 'Z') || (content[result.endIdx] >= '0' && content[result.endIdx] <= '9') || content[result.endIdx] == '_')
				{
					++result.endIdx;
				}
				result.word.append(content.begin() + result.startIdx, content.begin() + result.endIdx);
				break;
			}
			++result.endIdx;
		}

		return result;
	}


	void StructDefinitionParser()
	{
		std::cout << "Now Start struct parser:" << std::endl;

		std::string memberStructDefName;
		std::string memberVariableName;

		StructDefinition currentStructDef;
		WordAndIdx wordAndIdx = GetNextWord(charIdx, line);
		charIdx = wordAndIdx.endIdx;
		line = wordAndIdx.line;
		currentStructDef.structName = wordAndIdx.word;

		std::cout << "struct name: " << currentStructDef.structName << std::endl;

		token = content[charIdx];
		while (token!='}')
		{
			token = content[charIdx];
			if (token == '\n')
			{
				++line;
			}
			else if (token == '#')
			{
				while (content[charIdx] != '\n')
				{
					++charIdx;
				}
				++line;
			}
			else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
			{
				wordAndIdx = GetNextWord(charIdx, line);
				charIdx = wordAndIdx.endIdx;
				line = wordAndIdx.line;

				if (!wordAndIdx.word.compare("struct"))
				{
					StructDefinitionParser();

					wordAndIdx = GetNextWord(charIdx, line);
					charIdx = wordAndIdx.endIdx;
					line = wordAndIdx.line;
				}

				memberStructDefName = wordAndIdx.word;

				wordAndIdx = GetNextWord(charIdx, line);
				charIdx = wordAndIdx.endIdx;
				line = wordAndIdx.line;

				memberVariableName = wordAndIdx.word;

				currentStructDef.byteSize += structDefMap[memberStructDefName].byteSize;
				currentStructDef.membersMap[memberVariableName] = structDefMap[memberStructDefName];
				std::cout << memberStructDefName << " " << memberVariableName << std::endl;

				--charIdx;
			}

			++charIdx;
		}
		charIdx+=2;
		structDefMap[currentStructDef.structName] = currentStructDef;
		std::cout << "The bytesize of " << currentStructDef.structName << " is " << currentStructDef.byteSize << std::endl;
	}

	void SentenceParser()
	{

	}

	void FunctionDefinitionParser()
	{
		std::cout << "Now Start main function parser:" << std::endl;

		auto wordAndIdx = GetNextWord(charIdx,line);
		charIdx = wordAndIdx.endIdx;
		line = wordAndIdx.line;

		std::string returnTypeName;
		returnTypeName = wordAndIdx.word;
		StructDefinition returnValueStruct = structDefMap[returnTypeName];

		std::cout << returnTypeName << " ";


		FunctionDefinition currentDefinition;

		currentDefinition.returnTypeName = returnValueStruct.structName;
		
		wordAndIdx = GetNextWord(charIdx, line);
		charIdx = wordAndIdx.endIdx;
		line = wordAndIdx.line;

		std::string functionName = wordAndIdx.word;
		currentDefinition.functionName = functionName;

		std::cout << functionName << "(";


		while (content[charIdx]!='{')
		{

			if (content[charIdx] == '\n')
			{
				++line;
			}
			else if (content[charIdx] == '#')
			{
				while (content[charIdx] != '\n')
				{
					++charIdx;
				}
				++line;
			}
			else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
			{
				WordAndIdx wordAndIdx = GetNextWord(charIdx, line);
				charIdx = wordAndIdx.endIdx;
				line = wordAndIdx.line;

				std::string variableTypeName = wordAndIdx.word;

				wordAndIdx = GetNextWord(charIdx, line);
				charIdx = wordAndIdx.endIdx;
				line = wordAndIdx.line;

				std::string variableName = wordAndIdx.word;
				std::cout << variableTypeName << " " << variableName << ", " << std::endl;

				currentDefinition.localVariable[variableName] = structDefMap[variableTypeName];

				++currentDefinition.paramNum;

				--charIdx;
			}

			++charIdx;
		}
		std::cout << ");" << std::endl;

	}

	void MainFunctionParser()
	{
		std::cout << "Now Start main function parser:" << std::endl;

		while (content[charIdx] != '{')
		{
			++charIdx;
		}

		while (content[charIdx]!='}')
		{

		}

	}


	void Parser()
	{
		std::string word;

		while (charIdx < content.size())
		{
			token = content[charIdx];
			if (token == '\n')
			{
				++line;
			}
			else if (token == '#')
			{
				while (content[charIdx] != '\n')
				{
					++charIdx;
				}
				++line;
			}
			else if (token == '"')
			{
				++charIdx;
				unsigned long long lastIdx = charIdx;
				word.erase(word.begin(), word.end());

				while (content[charIdx] != '"')
				{
					++charIdx;
				}
				word.append(content.begin() + lastIdx, content.begin() + charIdx);

				std::cout << "Const string: " << word << std::endl;
			}
			else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_')
			{
				WordAndIdx wordAndIdx = GetNextWord(charIdx, line);
				word = wordAndIdx.word;

				if (!word.compare("struct"))
				{
					charIdx = wordAndIdx.endIdx;
					StructDefinitionParser();
				}
				else
				{
					std::string globalVariableTypeName = wordAndIdx.word;
					std::string globalVariableName;

					wordAndIdx = GetNextWord(wordAndIdx.endIdx, wordAndIdx.line);
					globalVariableName = wordAndIdx.word;

					unsigned long long lookAfterIdx = wordAndIdx.endIdx;

					while (content[lookAfterIdx] != ' ')
					{
						++lookAfterIdx;
					}

					if (content[lookAfterIdx] == '(')
					{
						FunctionDefinitionParser();
					}
					else
					{
						std::cout << "Global variable: " << globalVariableTypeName << " " << globalVariableName << std::endl;
						variableMap[globalVariableName] = structDefMap[globalVariableTypeName];
					}


				}

				--charIdx;

			}
			else if (token >= '0' && token <= '9')
			{
				unsigned long long lastIdx = charIdx;
				long long numValI = 0;
				double numValF = 0;
				bool isFloat = false;
				unsigned long long dotIdx = 0;

				word.erase(word.begin(), word.end());
				while ((content[charIdx] >= '0' && content[charIdx] <= '9') || content[charIdx] == '.')
				{
					if (content[charIdx] == '.')
					{
						isFloat = true;
					}
					++charIdx;
				}
				word.append(content.begin() + lastIdx, content.begin() + charIdx);
				--charIdx;

				if (isFloat)
				{
					numValF = std::stod(word);
					std::cout << "Const Float num: " << numValF << std::endl;
				}
				else
				{
					numValI = std::stoi(word);
					std::cout << "Const Int num: " << numValI << std::endl;
				}

			}
			else if (token == '+')
			{
				if (content[charIdx + 1] == '+')
				{
					++charIdx;
					std::cout << "Operator: ++" << std::endl;
				}
				else
				{
					std::cout << "Operator: +" << std::endl;
				}
			}
			else if (token == '-')
			{
				if (content[charIdx + 1] == '-')
				{
					++charIdx;
					std::cout << "Operator: --" << std::endl;
				}
				else
				{
					std::cout << "Operator: -" << std::endl;
				}
			}
			else if (token == '*')
			{
				std::cout << "Operator: *" << std::endl;
			}
			else if (token == '/')
			{
				if (content[charIdx + 1] == '/')
				{
					charIdx += 2;
					while (content[charIdx] != '\n')
					{
						++charIdx;
					}
					--charIdx;
					++line;

					std::cout << "Skip Commit" << std::endl;
				}
				else
				{
					std::cout << "Operator: /" << std::endl;
				}

			}
			else if (token == '=')
			{
				if (content[charIdx + 1] == '=')
				{
					++charIdx;

					std::cout << "Operator: ==" << std::endl;
				}
				else
				{
					std::cout << "Operator: =" << std::endl;
				}

			}
			else if (token == '!')
			{
				if (content[charIdx + 1] == '=')
				{
					++charIdx;

					std::cout << "Operator: !=" << std::endl;
				}
				else
				{
					std::cout << "Operator: !" << std::endl;
				}

			}
			else if (token == '<')
			{
				if (content[charIdx + 1] == '=')
				{
					++charIdx;

					std::cout << "Operator: <=" << std::endl;
				}
				else if (content[charIdx + 1] == '<')
				{
					++charIdx;

					std::cout << "Operator: <<" << std::endl;
				}
				else
				{
					std::cout << "Operator: <" << std::endl;
				}

			}
			else if (token == '>')
			{
				if (content[charIdx + 1] == '=')
				{
					++charIdx;

					std::cout << "Operator: >=" << std::endl;
				}
				else if (content[charIdx + 1] == '>')
				{
					++charIdx;

					std::cout << "Operator: >>" << std::endl;
				}
				else
				{
					std::cout << "Operator: >" << std::endl;
				}

			}
			else if (token == '|')
			{
				if (content[charIdx + 1] == '|')
				{
					++charIdx;

					std::cout << "Operator: ||" << std::endl;
				}
				else
				{
					std::cout << "Operator: |" << std::endl;
				}
			}
			else if (token == '&')
			{
				if (content[charIdx + 1] == '&')
				{
					++charIdx;

					std::cout << "Operator: &&" << std::endl;
				}
				else
				{
					std::cout << "Operator: &" << std::endl;
				}
			}
			else if (token == '^')
			{
				std::cout << "Operator: ^" << std::endl;

			}
			else if (token == '%')
			{
				std::cout << "Operator: %" << std::endl;
			}
			else if (token == '?')
			{
				std::cout << "Operator: ?" << std::endl;
			}

			++charIdx;
		}

	}

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

		std::cout << "Now compiling C file..." << path << std::endl;
		std::cout << "This content of current file is:" << std::endl;
		std::cout << content << std::endl << std::endl;

		std::cout << "Now start parser:" << std::endl;
		Parser();
		std::cout << "Now finished parser!" << std::endl << std::endl;

		std::cout << "Now generate instructions:" << std::endl;
		GenerateInstructions();
		std::cout << "Now finished generate instructions!" << std::endl << std::endl;

		std::cout << "Now compiling finished!" << std::endl;

	}
	void Execute()
	{
		std::cout << "Now execute program!" << std::endl;
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
			case EXIT: {std::cout << *(stackPointer - 1) << std::endl; }
					   break;
			default: {std::cout << "unknown instruction:%d\n" << *(stackPointer - 1); return; }
					 break;
			}

			++programCounter;

		}

		std::cout << "Now finished program execution!" << std::endl;
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
	std::string hierarchyPath = exePath.substr(0, exePath.find_last_of('\\'));
	std::string TinyCProgramSourceCodePath = hierarchyPath.substr(0, hierarchyPath.find_last_of('\\') + 1).append("TinyCSourceCode\\");

	std::string path = TinyCProgramSourceCodePath.append("program.c");
	TinyCProgram routine(8 * sizeof(long long), 8 * sizeof(long long));
	routine.CompileFile(path);

	routine.Execute();

}