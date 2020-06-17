#include "Token.h"

Token::Token(const Token::Type& type, const std::variant<float, int, char, std::string>& value) :
    type(type), 
    value(value)
{
}
