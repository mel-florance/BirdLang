#pragma once

#include <string>

#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "Interpreter.h"
#include "Context.h"
#include "Symbols.h"
#include "Platform.h"

class API Compiler {
public:
	Compiler();

	void interpret();

	std::string input;

	Context* context;
	Symbols* symbols;
	Lexer* lexer;
	Parser* parser;
	Interpreter* interpreter;
};