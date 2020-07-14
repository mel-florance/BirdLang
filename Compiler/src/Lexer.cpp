#include "pch.h"
#include <sstream>
#include "Lexer.h"
#include "Profiler.h"
#include "ConsoleTable.h"
#include "Utils.h"

using ConsoleTable = samilton::ConsoleTable;

const std::string digits = "0123456789";
const std::string letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string letters_digits = letters + digits;

Lexer::Lexer(const std::string& filename, bool debug) : 
	filename(filename),
	cursor(Cursor(-1, 0, -1, filename, input)),
	current_char('\0'),
	debug(false),
	lexing_time(0.0)
{
}

void Lexer::advance()
{
	cursor.advance(current_char);

	current_char = cursor.index < input.size()
		? input.at(cursor.index)
		: '\0';
}

std::vector<Token*> Lexer::index_tokens(const std::string& str)
{
	Profiler profiler;
	profiler.start = clock();
	this->input = str;
	std::vector<Token*> tokens;
	advance();

	while (current_char != '\0') {
		if (current_char == ' ' || current_char == '\t') {
			advance();
		}
		else if (digits.find(current_char) != std::string::npos) {
			tokens.push_back(create_numeric_token());
		}
		else if (letters.find(current_char) != std::string::npos) {
			tokens.push_back(create_identifier());
		}
		else if (current_char == '+') {
			tokens.push_back(new Token(Token::Type::PLUS, current_char));
			advance();
		}
		else if (current_char == '-') {
			tokens.push_back(new Token(Token::Type::MINUS, current_char));
			advance();
		}
		else if (current_char == '*') {
			tokens.push_back(new Token(Token::Type::MUL, current_char));
			advance();
		}
		else if (current_char == '%') {
			tokens.push_back(new Token(Token::Type::MOD, current_char));
			advance();
		}
		else if (current_char == '/') {
			tokens.push_back(new Token(Token::Type::DIV, current_char));
			advance();
		}
		else if (current_char == '^') {
			tokens.push_back(new Token(Token::Type::POW, current_char));
			advance();
		}
		else if (current_char == '(') {
			tokens.push_back(new Token(Token::Type::LPAREN, current_char));
			advance();
		}
		else if (current_char == ')') {
			tokens.push_back(new Token(Token::Type::RPAREN, current_char));
			advance();	
		}
		else if (current_char == '!') {
			Token* token = create_not_equals_operator();

			if (token == nullptr) {
				Cursor start = Cursor(cursor);
				advance();
				
				ExpectedCharacterError* error = new ExpectedCharacterError(
					start, 
					cursor, 
					"'=' (after '!')"
				);
				
				std::cout << error << "\n";

				return std::vector<Token*>();
			}

			tokens.push_back(token);
		}
		else if (current_char == '=') {
			tokens.push_back(create_equals_operator());
		}
		else if (current_char == '<') {
			tokens.push_back(create_less_operator());
		}
		else if (current_char == '>') {
			tokens.push_back(create_greater_operator());
		}
		else {
			Cursor start = Cursor(cursor);
			char c = current_char;
			advance();

			IllegarCharError* error = new IllegarCharError(
				start, 
				cursor,
				std::string(1, c)
			);
			
			std::cout << error << "\n";

			return std::vector<Token*>();
		}
	}

	tokens.push_back(new Token(Token::Type::EOT, char(0x04), &cursor));
	profiler.end = clock();
	lexing_time = profiler.getReport();

	if (debug) {
		ConsoleTable table(1, 2);
		ConsoleTable::TableChars chars;

		chars.topDownSimple = '-';
		chars.leftRightSimple = '|';
		
		chars.leftSeparation = '+';
		chars.rightSeparation= '+';
		chars.centreSeparation= '+';
		chars.topSeparation= '+';
		chars.downSeparation = '+';

		table.setTableChars(chars);

		table[0][0] = "Identifier";
		table[0][1] = "Character";
		table[0][2] = "Line";
		table[0][3] = "Column";

		for (unsigned int i = 0; i < tokens.size(); i++) {
			auto token = tokens.at(i);
			std::stringstream str;

			switch (token->value.index()) {
			case 0:
				try { str << std::get<float>(token->value); }
				catch (const std::bad_variant_access&) {}
				break;
			case 1:
				try { str << std::get<int>(token->value); }
				catch (const std::bad_variant_access&) {}
				break;
			case 2:
				try { str << std::get<char>(token->value); }
				catch (const std::bad_variant_access&) {}
				break;
			case 3:
				try { str << std::get<std::string>(token->value); }
				catch (const std::bad_variant_access&) {}
				break;
			}
			
			table[i + 1][0] = Token::toString(token->type);
			table[i + 1][1] = str.str();

			int line = 0;
			int column = 0;

			if (token->start != nullptr) {
				line = token->start->line;
				column = token->start->column;
			}
				
			table[i + 1][2] = std::to_string(line);
			table[i + 1][3] = std::to_string(column);
		}

		Utils::title("TOKENS");
		std::cout << table << "\n";
	}

	return tokens;
}

Token* Lexer::create_numeric_token()
{
	std::string str;
	unsigned int dots = 0;
	Cursor* start = new Cursor(cursor);
	auto numbers = digits + ".";

	auto hasDot = [=]() { 
		return numbers.find(current_char) != std::string::npos; 
	};

	while (current_char != '\0' && hasDot()) {
		if (current_char == '.') {
			
			if (dots == 1)
				break;

			++dots;
			str += '.';
		}
		else {
			str += current_char;
		}

		advance();
	}

	if (dots == 0) {
		return new Token(
			Token::Type::INT, 
			(int)std::atoi(str.c_str()), 
			start,
			&cursor
		);
	}
	else {
		return new Token(
			Token::Type::FLOAT,
			(float)std::atof(str.c_str()),
			start, 
			&cursor
		);
	}
}

Token* Lexer::create_identifier()
{
	std::string id;
	Cursor* start = new Cursor(cursor);
	auto str = letters_digits + "_";

	auto isLetterOrDigit = [=]() { 
		return str.find(current_char) != std::string::npos;
	};

	while (current_char != '\0' && isLetterOrDigit()) {
		id += current_char;
		advance();
	}

	Token::Type type = Token::Type::NONE;

	std::vector<std::string>::iterator it = std::find(
		Token::keywords.begin(), 
		Token::keywords.end(), 
		id
	);

	type = it != Token::keywords.end()
		? Token::Type::KEYWORD
		: Token::Type::IDENTIFIER;

	return new Token(type, id, start, &cursor);
}

Token* Lexer::create_equals_operator()
{
	Token::Type type = Token::Type::EQ;
	std::string value = "=";
	Cursor* start = new Cursor(cursor);

	advance();

	if (current_char == '=') {
		advance();
		type = Token::Type::EE;
		value = "==";
	}

	return new Token(type, value, start, &cursor);
}

Token* Lexer::create_not_equals_operator()
{
	Cursor* start = new Cursor(cursor);
	advance();

	if (current_char == '=') {
		advance();
		return new Token(Token::Type::NE, "!=", start, &cursor);
	}

	advance();
	return nullptr;
}

Token* Lexer::create_less_operator()
{
	Token::Type type = Token::Type::LT;
	std::string value = "<";
	Cursor* start = new Cursor(cursor);
	advance();

	if (current_char == '=') {
		advance();
		type = Token::Type::LTE;
		value = "<=";
	}

	return new Token(type, value, start, &cursor);
}

Token* Lexer::create_greater_operator()
{
	Token::Type type = Token::Type::GT;
	std::string value = ">";
	Cursor* start = new Cursor(cursor);
	advance();

	if (current_char == '=') {
		advance();
		type = Token::Type::GTE;
		value = ">=";
	}

	return new Token(type, value, start, &cursor);
}
