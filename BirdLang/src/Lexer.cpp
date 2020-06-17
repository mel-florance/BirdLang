#include "Lexer.h"

const std::string digits = "0123456789";

Lexer::Lexer(const std::string& filename, const std::string& input) : 
	filename(filename),
	input(input),
	cursor(Cursor(-1, 0, -1, filename, input)),
	current_char(NULL)
{
	advance();
}

void Lexer::advance()
{
	cursor.advance(current_char);

	current_char = cursor.index < input.size()
		? input.at(cursor.index)
		: NULL;
}

std::vector<Token> Lexer::make_tokens()
{
	std::vector<Token> tokens;

	while (current_char != NULL) {
		if (current_char == ' ' || current_char == '\t') {
			advance();
		}
		else if (digits.find(current_char) != std::string::npos) {
			tokens.push_back(make_number());
		}
		else if (current_char == '+') {
			tokens.push_back(Token(Token::Type::PLUS, current_char));
			advance();
		}
		else if (current_char == '-') {
			tokens.push_back(Token(Token::Type::MINUS, current_char));
			advance();
		}
		else if (current_char == '*') {
			tokens.push_back(Token(Token::Type::MUL, current_char));
			advance();
		}
		else if (current_char == '/') {
			tokens.push_back(Token(Token::Type::DIV, current_char));
			advance();
		}
		else if (current_char == '(') {
			tokens.push_back(Token(Token::Type::LPAREN, current_char));
			advance();
		}
		else if (current_char == ')') {
			tokens.push_back(Token(Token::Type::RPAREN, current_char));
			advance();
		}
		else {
			Cursor start = cursor.copy();
			char c = current_char;
			advance();

			IllegarCharError error(start, cursor, std::string(1, c));
			std::cout << error << "\n";

			return std::vector<Token>();
		}
	}

	return tokens;
}

Token Lexer::make_number()
{
	std::string str;
	unsigned int dot_count = 0;

	while (current_char != NULL && (digits + ".").find(current_char) != std::string::npos) {
		if (current_char == '.') {
			
			if (dot_count == 1)
				break;

			++dot_count;
			str += '.';
		}
		else {
			str += current_char;
		}

		advance();
	}

	if (dot_count == 0) {
		return Token(Token::Type::INT, (int)std::atoi(str.c_str()));
	}
	else {
		return Token(Token::Type::FLOAT, (float)std::atof(str.c_str()));
	}
}
