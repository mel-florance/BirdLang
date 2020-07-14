#pragma once

#include <string>

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

	Context* context;
	Symbols* symbols;

	bool debug_lexer;
	bool debug_parser;
	bool profiling;
};