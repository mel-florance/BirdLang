#include "pch.h"
#include "Compiler.h"

int main()
{
	std::cout << "Bird Lang Interpreter:\n";
	Compiler compiler;
	compiler.interpret();
}