#pragma once

#include "Token.h"
#include "Error.h"
#include "Cursor.h"
#include "Platform.h"

class Lexer
{
public:
	Lexer(const std::string& filename, bool debug = false);
	
	void advance();
	std::vector<Token*> index_tokens(const std::string& str);
	Token* create_numeric_token();
	Token* create_identifier();
	Token* create_equals_operator();
	Token* create_not_equals_operator();
	Token* create_less_operator();
	Token* create_greater_operator();

	std::string filename;
	std::string input;
	Cursor cursor;
	char current_char;
	bool debug;
};

