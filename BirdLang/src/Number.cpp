#include "Number.h"

Number::Number(const std::variant<float, int>& value) :
    value(value),
    start(nullptr),
    end(nullptr) {
}

std::pair<Number*, Error*> Number::add(Number* other)
{
    Number* result = new Number();
    
    if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = std::get<float>(value) + std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
    }
    else if(value.index() == 1 && other->value.index() == 1) {
		try { result->value = std::get<int>(value) + std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
    }
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = std::get<float>(value) + (float)std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)std::get<int>(value) + std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

    return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::subtract(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = std::get<float>(value) - std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = std::get<int>(value) - std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = std::get<float>(value) - (float)std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)std::get<int>(value) - std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::multiply(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = std::get<float>(value) * std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = std::get<int>(value) * std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = std::get<float>(value) * (float)std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)std::get<int>(value) * std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::divide(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0 && other->value.index() == 0) {
		try { 
			auto rhs = std::get<float>(other->value);

			if (rhs == 0.0f)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero"));
			else
				result->value = std::get<float>(value) / rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { 
			auto rhs = std::get<int>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero"));
			else
				result->value = std::get<int>(value) / rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try {
			auto rhs = std::get<int>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero"));
			else
				result->value = std::get<float>(value) / (float)rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try {
			auto rhs = std::get<float>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero"));
			else
				result->value = (float)std::get<int>(value) / rhs;
		}
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}
