#include "Lexer.h"

const std::string digits = "0123456789";

Lexer::Lexer(const std::string& filename, const std::string& input) : 
	filename(filename),
	input(input),
	cursor(Cursor(-1, 0, -1, filename, input)),
	current_char(NULL),
	debug(true)
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

std::vector<Token*> Lexer::index_tokens()
{
	std::vector<Token*> tokens;

	while (current_char != NULL) {
		if (current_char == ' ' || current_char == '\t') {
			advance();
		}
		else if (digits.find(current_char) != std::string::npos) {
			tokens.push_back(create_numeric_token());
		}
		else if (current_char == '+') {
			tokens.push_back(new Token(Token::Type::PLUS, current_char));
			advance();
		}
		else if (current_char == '-') {
			tokens.push_back(new Token(Token::Type::MINUS, current_char));
			advance();
		}
		else if (current_char == '^') {
			tokens.push_back(new Token(Token::Type::POW, current_char));
			advance();
		}
		else if (current_char == '*') {
			tokens.push_back(new Token(Token::Type::MUL, current_char));
			advance();
		}
		else if (current_char == '/') {
			tokens.push_back(new Token(Token::Type::DIV, current_char));
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
		else {
			Cursor start = Cursor(cursor);
			char c = current_char;
			advance();

			IllegarCharError* error = new IllegarCharError(start, cursor, std::string(1, c));
			std::cout << error << "\n";

			return std::vector<Token*>();
		}
	}

	tokens.push_back(new Token(Token::Type::EOT, 0, &cursor));

	if (debug) {
		for (auto token : tokens) {
			std::cout << token << "\n";
		}
	}

	return tokens;
}

Token* Lexer::create_numeric_token()
{
	std::string str;
	unsigned int dots = 0;
	Cursor* start = new Cursor(cursor);

	auto hasDot = [=]() { return (digits + ".").find(current_char) != std::string::npos; };

	while (current_char != NULL && hasDot()) {
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
		return new Token(Token::Type::INT, (int)std::atoi(str.c_str()), start, &cursor);
	}
	else {
		return new Token(Token::Type::FLOAT, (float)std::atof(str.c_str()), start, &cursor);
	}
}
