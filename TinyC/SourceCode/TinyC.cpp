#include<fstream>
#include<iostream>
#include<string>
#include <sstream>
#include<cmath>

int token;
char* src, *old_src;

int poolSize;
int line;


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

int eval() {
	//Do nothing yet
	return 0;
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

}