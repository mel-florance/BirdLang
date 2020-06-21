#include "pch.h"
#include "Token.h"

std::vector<std::string> Token::keywords = {
    "var"
};

Token::Token(const Token::Type& type, const std::variant<float, int, char, std::string>& value, Cursor* start, Cursor* end) :
    type(type), 
    value(value),
    start(start),
    end(end)
{
    if (start != nullptr) {
        this->start = new Cursor(start);
        this->end = new Cursor(start);
        this->end->advance();
    }

	if (end != nullptr) {
		this->end = new Cursor(end);
	}
}
