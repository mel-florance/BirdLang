#include <iostream>
#include <BirdLang.h>

int main() {
	std::cout << "BirdLang Interpreter" << std::endl;

	Compiler compiler;
	compiler.interpret();

	return 0;
}