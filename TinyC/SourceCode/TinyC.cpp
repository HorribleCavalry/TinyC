#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>

int token;
int token_val;
char* src, *old_src;

const int poolSize = 1024 * 256;
int line;

int *text,
*old_text,
*stack;
char *data;

int* pc, *bp, *sp, ax, cycle;

int* current_id;
int* symbols;
int *idmain;
enum Instruction
{
	NONE, LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
	OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
	OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
};

enum Identifier
{
	Token, Hash, Name, Type, Class, Value, BType, BClass, BValue, IdSize
};

enum { CHAR, INT, PTR };

enum
{
	Num = 128, Fun, Sys, Glo, Loc, Id,
	Char, Else, Enum, If, Int, Return, Sizeof, While,
	Assign, Cond, Lor, Lan, Or, Xor, And, Eq, Ne, Lt, Gt, Le, Ge, Shl, Shr, Add, Sub, Mul, Div, Mod, Inc, Dec, Brak
};

struct identifier
{
	int token;
	int hash;
	char* name;
	int Class;
	int type;
	int value;
	int Bclass;
	int Btype;
	int Bvalue;
};


void next() {
	char *last_pos;
	int hash;

	while (token = *src) {
		++src;

		// parse token here
		if (token == '\n') {
			++line;
		}
		else if (token == '#') {
			// skip macro, because we will not support it
			while (*src != 0 && *src != '\n') {
				src++;
			}
		}
		else if ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || (token == '_')) {

			// parse identifier
			last_pos = src - 1;
			hash = token;

			while ((*src >= 'a' && *src <= 'z') || (*src >= 'A' && *src <= 'Z') || (*src >= '0' && *src <= '9') || (*src == '_')) {
				hash = hash * 147 + *src;
				src++;
			}

			// look for existing identifier, linear search
			current_id = symbols;
			while (current_id[Token]) {
				if (current_id[Hash] == hash && !memcmp((char *)current_id[Name], last_pos, src - last_pos)) {
					//found one, return
					token = current_id[Token];
					return;
				}
				current_id = current_id + IdSize;
			}


			// store new ID
			current_id[Name] = (int)last_pos;
			current_id[Hash] = hash;
			token = current_id[Token] = Id;
			return;
		}
		else if (token >= '0' && token <= '9') {
			// parse number, three kinds: dec(123) hex(0x123) oct(017)
			token_val = token - '0';
			if (token_val > 0) {
				// dec, starts with [1-9]
				while (*src >= '0' && *src <= '9') {
					token_val = token_val * 10 + *src++ - '0';
				}
			}
			else {
				// starts with 0
				if (*src == 'x' || *src == 'X') {
					//hex
					token = *++src;
					while ((token >= '0' && token <= '9') || (token >= 'a' && token <= 'f') || (token >= 'A' && token <= 'F')) {
						token_val = token_val * 16 + (token & 15) + (token >= 'A' ? 9 : 0);
						token = *++src;
					}
				}
				else {
					// oct
					while (*src >= '0' && *src <= '7') {
						token_val = token_val * 8 + *src++ - '0';
					}
				}
			}

			token = Num;
			return;
		}
		else if (token == '"' || token == '\'') {
			// parse string literal, currently, the only supported escape
			// character is '\n', store the string literal into data.
			last_pos = data;
			while (*src != 0 && *src != token) {
				token_val = *src++;
				if (token_val == '\\') {
					// escape character
					token_val = *src++;
					if (token_val == 'n') {
						token_val = '\n';
					}
				}

				if (token == '"') {
					*data++ = token_val;
				}
			}

			src++;
			// if it is a single character, return Num token
			if (token == '"') {
				token_val = (int)last_pos;
			}
			else {
				token = Num;
			}

			return;
		}
		else if (token == '/') {
			if (*src == '/') {
				// skip comments
				while (*src != 0 && *src != '\n') {
					++src;
				}
			}
			else {
				// divide operator
				token = Div;
				return;
			}
		}
		else if (token == '=') {
			// parse '==' and '='
			if (*src == '=') {
				src++;
				token = Eq;
			}
			else {
				token = Assign;
			}
			return;
		}
		else if (token == '+') {
			// parse '+' and '++'
			if (*src == '+') {
				src++;
				token = Inc;
			}
			else {
				token = Add;
			}
			return;
		}
		else if (token == '-') {
			// parse '-' and '--'
			if (*src == '-') {
				src++;
				token = Dec;
			}
			else {
				token = Sub;
			}
			return;
		}
		else if (token == '!') {
			// parse '!='
			if (*src == '=') {
				src++;
				token = Ne;
			}
			return;
		}
		else if (token == '<') {
			// parse '<=', '<<' or '<'
			if (*src == '=') {
				src++;
				token = Le;
			}
			else if (*src == '<') {
				src++;
				token = Shl;
			}
			else {
				token = Lt;
			}
			return;
		}
		else if (token == '>') {
			// parse '>=', '>>' or '>'
			if (*src == '=') {
				src++;
				token = Ge;
			}
			else if (*src == '>') {
				src++;
				token = Shr;
			}
			else {
				token = Gt;
			}
			return;
		}
		else if (token == '|') {
			// parse '|' or '||'
			if (*src == '|') {
				src++;
				token = Lor;
			}
			else {
				token = Or;
			}
			return;
		}
		else if (token == '&') {
			// parse '&' and '&&'
			if (*src == '&') {
				src++;
				token = Lan;
			}
			else {
				token = And;
			}
			return;
		}
		else if (token == '^') {
			token = Xor;
			return;
		}
		else if (token == '%') {
			token = Mod;
			return;
		}
		else if (token == '*') {
			token = Mul;
			return;
		}
		else if (token == '[') {
			token = Brak;
			return;
		}
		else if (token == '?') {
			token = Cond;
			return;
		}
		else if (token == '~' || token == ';' || token == '{' || token == '}' || token == '(' || token == ')' || token == ']' || token == ',' || token == ':') {
			// directly return the character as token;
			return;
		}
	}
	return;
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

	line = 1;

	text = old_text = (int*)malloc(poolSize);
	data = (char*)malloc(poolSize);
	stack = (int*)malloc(poolSize);
	symbols = (int*)malloc(poolSize);
	src = old_src = (char*)malloc(content.size()+1);


	memset(text, 0, poolSize);
	memset(data, 0, poolSize);
	memset(stack, 0, poolSize);
	memset(symbols, 0, poolSize);

	bp = sp = (int*)((int)stack + poolSize);
	ax = 0;

	src = new char[101];
	const char* Ssrc = "char else enum if int return sizeof while open read close printf malloc memset memcmp exit void main";
	memcpy(src, Ssrc, 101 * sizeof(char));
	//printf("%s", src);
	int i = Char;

	memcpy(old_src, content.c_str(), (content.size() + 1) * sizeof(char));
	src = old_src;
	printf("%s", old_src);

	while (i <= While)
	{
		next();
		current_id[Token] = i++;
	}

	i = OPEN;
	while (i <= EXIT)
	{
		next();
		current_id[Class] = Sys;
		current_id[Type] = INT;
		current_id[Value] = i++;
	}

	next();
	current_id[Token] = Char;
	next();
	idmain = current_id;


	pc = text;
	program();

	return eval();
}