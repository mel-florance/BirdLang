#pragma once

#include "Cursor.h"
#include "Platform.h"

#include <vector>
#include <variant>

class Token
{
public:
	enum class Type {
		INT,
		DOUBLE,
		STRING,
		PLUS,
		MINUS,
		POW,
		MUL,
		DIV,
		MOD,
		LPAREN,
		RPAREN,
		IDENTIFIER,
		KEYWORD,
		COMMA,
		ARROW,
		EQ,
		EE,
		NE,
		LT,
		GT,
		LTE,
		GTE,
		EOL,
		NONE
	};

	static std::vector<std::string> keywords;

	Token(Token* token) {
		type = token->type;
		value = token->value;
		start = token->start;
		end = token->end;
	}

	Token(
		const Type& type = Type::NONE,
		const std::variant<double, int, char, std::string>& value = '0',
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr
	);

	static inline std::string toString(Type type) {
		switch (type) {
		case Type::INT: return "INT";
		case Type::DOUBLE: return "DOUBLE";
		case Type::STRING: return "STRING";
		case Type::PLUS: return "PLUS";
		case Type::MINUS: return "MINUS";
		case Type::POW: return "POW";
		case Type::MUL: return "MUL";
		case Type::MOD: return "MOD";
		case Type::DIV: return "DIV";
		case Type::LPAREN: return "LPAREN";
		case Type::RPAREN: return "RPAREN";
		case Type::IDENTIFIER: return "IDENTIFIER";
		case Type::KEYWORD: return "KEYWORD";
		case Type::COMMA: return "COMMA";
		case Type::ARROW: return "ARROW";
		case Type::EQ: return "EQ";
		case Type::EE: return "EE";
		case Type::NE: return "NE";
		case Type::LT: return "LT";
		case Type::GT: return "GT";
		case Type::LTE: return "LTE";
		case Type::GTE: return "GTE";
		case Type::EOL: return "EOL";
		case Type::NONE: return "NONE";
		}

		return "UNDEFINED";
	}

	inline friend std::ostream& operator << (std::ostream& stream, Token* token) {
		auto token_type = toString(token->type) + ": ";
		stream << token_type;

		switch (token->value.index()) {
		case 0:
			try {
				stream << std::get<double>(token->value);
			}
			catch (const std::bad_variant_access&) {}
			break;
		case 1:
			try {
				stream << std::get<int>(token->value);
			}
			catch (const std::bad_variant_access&) {}
			break;
		case 2:
			try {
				stream << std::get<char>(token->value);
			}
			catch (const std::bad_variant_access&) {}
			break;
		case 3:
			try {
				stream << std::get<std::string>(token->value);
			}
			catch (const std::bad_variant_access&) {}
			break;
		}

		return stream;
	}

	Type type;
	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	std::variant<double, int, char, std::string> value;
};
