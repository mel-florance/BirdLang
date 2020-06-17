#pragma once

#include <iostream>
#include <variant>
#include <string>

class Token
{
public:
	enum class Type {
		INT,
		FLOAT,
		STRING,
		PLUS,
		MINUS,
		MUL,
		DIV,
		LPAREN,
		RPAREN
	};

	Token(const Type& type, const std::variant<float, int, char, std::string>& value);

	static inline std::string toString(Type type) {
		switch (type) {
		case Type::INT: return "INT";
		case Type::FLOAT: return "FLOAT";
		case Type::STRING: return "STRING";
		case Type::PLUS: return "PLUS";
		case Type::MINUS: return "MINUS";
		case Type::MUL: return "MUL";
		case Type::DIV: return "DIV";
		case Type::LPAREN: return "LAPREN";
		case Type::RPAREN: return "RPAREN";
		}

		return "UNDEFINED";
	}

	inline friend std::ostream& operator << (std::ostream& stream, const Token& token) {
		auto token_type = toString(token.type) + ": ";

		switch (token.type) {
		default:
			stream << token_type << std::get<char>(token.value);
			break;
		case Type::INT:
			stream << token_type << std::get<int>(token.value);
			break;
		case Type::FLOAT:
			stream << token_type << std::get<float>(token.value);
			break;
		case Type::STRING:
			stream << token_type << std::get<std::string>(token.value);
			break;
		}

		return stream;
	}

	Type type;
	std::variant<float, int, char, std::string> value;
};
