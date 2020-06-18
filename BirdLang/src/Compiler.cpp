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
		lexer.debug = false;
		auto tokens = lexer.index_tokens();

		// Generate AST
		Parser parser(tokens);
		parser.debug = false;
		auto ast = parser.parse();

		if (ast != nullptr) {
			if (ast->error != nullptr)
				std::cout << ast->error << std::endl;
			else {
				// Interpret the AST
				Interpreter interpreter;
				auto result = interpreter.visit(ast->node);

				if (result->error != nullptr) {
					std::cout << result->error << std::endl;
				}
				else {
					std::cout << result->value << std::endl;
				}
			}
		}
	}
}