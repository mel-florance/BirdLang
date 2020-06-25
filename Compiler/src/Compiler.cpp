#include "pch.h"

#include "Compiler.h"

Compiler::Compiler()
{
	context = new Context("<program>");
	symbols = new Symbols();
	symbols->set("null", 0);
	context->symbols = symbols;

	lexer = new Lexer("<stdin>");
}

void Compiler::interpret()
{
	while (true)
	{
		// Get user input
		std::cout << "\n" << "> ";
		std::getline(std::cin >> std::ws, input);

		// Clear the console if asked
		if (input == "clear") {
			system("cls");
			continue;
		}

		// Index tokens
		lexer->input = input;
		lexer->debug = false;
		auto tokens = lexer->index_tokens();

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

				auto result = interpreter.visit(ast->node, context);

				if (result->error != nullptr) {
					if (strcmp(typeid(*result->error).name(), "class RuntimeError") == 0) {
						RuntimeError* error = static_cast<RuntimeError*>(result->error);

						if (error != nullptr) {
							std::cout << error << std::endl;
						}
					}
					else
						std::cout << result->error << std::endl;
				}
				else {
					std::cout << result->value << std::endl;
				}
			}
		}
	}
}
