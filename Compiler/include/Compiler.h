#pragma once

#include <string>
#include <memory>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Context.h"
#include "Symbols.h"
#include "Platform.h"

class Compiler {
public:
	Compiler();

	void interpret(const std::string& input);
	void printStatistics();

	Context* context;
	Symbols* symbols;

	std::unique_ptr<Lexer> lexer;
	std::unique_ptr<Parser> parser;
	std::unique_ptr<Interpreter> interpreter;

	bool debug_lexer;
	bool debug_parser;
	bool profiling;

	double interpreting_time;
};
