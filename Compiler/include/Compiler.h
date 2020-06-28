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

	std::shared_ptr<Context> context;
	std::shared_ptr<Symbols> symbols;

	bool debug_lexer;
	bool debug_parser;
};