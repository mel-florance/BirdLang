#include <iostream>
#include <string>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"

int main()
{
	std::cout << "Bird Lang Interpreter:\n";

	std::string input;

	while (true)
	{
		// Get user input
		std::cout << "\n" << "> ";
		std::getline(std::cin >> std::ws, input);

		// Index tokens
		Lexer lexer("<stdin>", input);
		auto tokens = lexer.index_tokens();

		for (auto token : tokens) {
			std::cout << token << "\n";
		}

		// Generate AST
		Parser parser(tokens);
		auto ast = parser.parse();

		if (ast != nullptr) {
			if (ast->error != nullptr)
				std::cout << ast->error << std::endl;
			else {
				// Interpret the AST
				Interpreter interpreter;
				interpreter.visit(ast->node);
			}
		}
	}
}