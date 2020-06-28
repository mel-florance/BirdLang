#include "pch.h"

#include "Compiler.h"

Compiler::Compiler()
{
	context = std::make_shared<Context>("<program>");
	symbols = std::make_shared<Symbols>();

	symbols->set("null", NULL);
	symbols->set("true", true);
	symbols->set("false", false);

	symbols->set("PI", PI);
	symbols->set("TAU", TAU);

	context->symbols = symbols;
}

void Compiler::interpret(const std::string& input)
{
	Lexer lexer("<stdin>");
	lexer.debug = false;
	auto tokens = lexer.index_tokens(input);

	Parser parser;
	parser.debug = false;
	parser.setTokens(tokens);
	auto ast = parser.parse();

	if (ast != nullptr) {
		if (ast->error != nullptr) {
			std::cout << ast->error << std::endl;
		}
		else {
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
