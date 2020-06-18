#pragma once

#include <variant>
#include <iostream>

#include "Cursor.h"

class Number {
public:
	Number(const std::variant<float, int>& value = 0);

	Number add(const Number& other);
	Number subtract(const Number& other);
	Number multiply(const Number& other);
	Number divide(const Number& other);

	inline friend std::ostream& operator << (std::ostream& stream, const Number& number) {

		if (number.value.index() == 0) {
			try { stream << std::to_string(std::get<float>(number.value)); }
			catch (const std::bad_variant_access&) {}
		}
		else {
			try { stream << std::to_string(std::get<int>(number.value)); }
			catch (const std::bad_variant_access&) {}
		}

		return stream;
	}

	std::variant<float, int> value;
	Cursor* start;
	Cursor* end;
};

