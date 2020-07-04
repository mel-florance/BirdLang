#include "pch.h"

#include "Compiler.h"

Compiler::Compiler() :
	debug_lexer(false),
	debug_parser(false)
{
	context = std::make_shared<Context>("<program>");
	symbols = std::make_shared<Symbols>();

	symbols->set("null", 0);
	symbols->set("true", true);
	symbols->set("false", false);

	symbols->set("PI", PI);
	symbols->set("TAU", TAU);
	symbols->set("PHI", 1.618033988749895f);
	symbols->set("EULER", 2.718281828459045f);
	symbols->set("SQRT1_2", 0.7071067811865476f);
	symbols->set("SQRT2", 1.4142135623730951f);

	context->symbols = symbols;
}

void Compiler::interpret(const std::string& input)
{
	Lexer lexer("<stdin>");
	lexer.debug = debug_lexer;
	auto tokens = lexer.index_tokens(input);

	Parser parser;
	parser.debug = debug_parser;e
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
