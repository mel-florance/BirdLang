#pragma once

#include <variant>
#include "Cursor.h"

class Number {
public:
	Number(const std::variant<float, int>& value = 0);

	Number add(const Number& other);
	Number subtract(const Number& other);
	Number multiply(const Number& other);
	Number divide(const Number& other);

	std::variant<float, int> value;
	Cursor* start;
	Cursor* end;
};

