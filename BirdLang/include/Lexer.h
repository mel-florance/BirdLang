#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Token.h"
#include "Error.h"
#include "Cursor.h"

class Lexer
{
public:
	Lexer(const std::string& filename, const std::string& input);
	
	void advance();
	std::vector<Token*> index_tokens();
	Token* create_numeric_token();
	Token* create_identifier();

	std::string filename;
	std::string input;
	Cursor cursor;
	char current_char;
	bool debug;
};

