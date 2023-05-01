#include "LexAnalyser.h"
#include "SyntaxAnalyser.h"
#include "CodeGen.h"
#include <iostream>

int main(int argc, char* argv[])
{
	lexAnalysis(argv[1]);
	syntaxAnalysis();
	genTriads();
	std::cout << "Компиляция успешно завершена!" << std::endl;
	return 0;
}
