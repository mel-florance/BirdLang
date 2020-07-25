#include "pch.h"
#include "Token.h"

std::vector<std::string> Token::keywords = {
    "var",
    "and",
    "or",
    "not",
    "if",
    "then",
    "else",
    "else if",
    "for",
    "while",
    "to",
    "step",
	"fn"
};

Token::Token(
	const Token::Type& type,
	const std::variant<float, int, char, std::string>& value,
	std::shared_ptr<Cursor> start,
	std::shared_ptr<Cursor> end
) :
    type(type), 
    value(value),
    start(start),
    end(end)
{
    if (start != nullptr) {
        this->start = start;
        this->end = start;
        this->end->advance();
    }

	if (end != nullptr) {
		this->end = end;
	}
}
