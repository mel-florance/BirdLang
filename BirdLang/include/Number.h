#pragma once

#include <variant>
#include <iostream>

#include "Cursor.h"
#include "Error.h"

class Number {
public:
	Number(const std::variant<float, int>& value = 0);

	std::pair<Number*, Error*> add(Number* other);
	std::pair<Number*, Error*> subtract(Number* other);
	std::pair<Number*, Error*> multiply(Number* other);
	std::pair<Number*, Error*> divide(Number* other);

	inline friend std::ostream& operator << (std::ostream& stream, Number* number) {

		if (number->value.index() == 0) {
			try { stream << std::to_string(std::get<float>(number->value)); }
			catch (const std::bad_variant_access&) {}
		}
		else {
			try { stream << std::to_string(std::get<int>(number->value)); }
			catch (const std::bad_variant_access&) {}
		}

		return stream;
	}

	std::variant<float, int> value;
	Cursor* start;
	Cursor* end;
};
