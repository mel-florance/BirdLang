#include <iostream>
#include <string>

#include "Token.h"
#include "Lexer.h"

int main()
{
	std::cout << "Bird Lang Interpreter:\n";

	std::string input;

	while (true)
	{
		std::cout << "\n" << "> ";
		std::getline(std::cin >> std::ws, input);

		Lexer lexer("<stdin>", input);
		auto tokens = lexer.index_tokens();

		for (auto token : tokens) {
			std::cout << token << "\n";
		}
	}
}