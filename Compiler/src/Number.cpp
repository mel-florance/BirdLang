#include "pch.h"
#include "Number.h"

Number::Number(const std::variant<double, int, bool, Function*>& value) :
	Type(value)
{
}

std::pair<Number*, Error*> Number::add(Type* other)
{
    Number* result = new Number();
	result->context = context;
    
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

std::pair<Number*, Error*> Number::subtract(Type* other)
{
	Number* result = new Number();
	result->context = context;

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

std::pair<Number*, Error*> Number::multiply(Type* other)
{
	Number* result = new Number();
	result->context = context;

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

std::pair<Number*, Error*> Number::modulus(Type* other)
{
	Number* result = new Number();
	result->context = context;

	if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = std::get<int>(value) % std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else
		return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Modulus supports only integers.", context));

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::divide(Type* other)
{
	Number* result = new Number();
	result->context = context;

	if (value.index() == 0 && other->value.index() == 0) {
		try { 
			auto rhs = std::get<float>(other->value);

			if (rhs == 0.0f)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero", context));
			else
				result->value = std::get<float>(value) / rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { 
			auto rhs = std::get<int>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero", context));
			else
				result->value = (float)std::get<int>(value) / (float)rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try {
			auto rhs = std::get<int>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero", context));
			else
				result->value = std::get<float>(value) / (float)rhs;
		}
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try {
			auto rhs = std::get<float>(other->value);

			if (rhs == 0)
				return std::make_pair(nullptr, new RuntimeError(other->start, other->end, "Division by zero", context));
			else
				result->value = (float)std::get<int>(value) / rhs;
		}
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::power(Type* other)
{
	Number* result = new Number();
	result->context = context;

	if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = (float)pow(std::get<float>(value), std::get<float>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = (float)pow((float)std::get<int>(value), std::get<int>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = (float)pow(std::get<float>(value), (float)std::get<int>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)pow((float)std::get<int>(value), std::get<float>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_equal(Type* other)
{
	Number* result = new Number();
	result->value = (bool)(value == other->value);
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_not_equal(Type* other)
{
	Number* result = new Number();
	result->value = value != other->value;
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_less_than(Type* other)
{
	Number* result = new Number();
	result->value = value < other->value;
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_greater_than(Type* other)
{
	Number* result = new Number();
	result->value = value > other->value;
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_less_or_equal(Type* other)
{
	Number* result = new Number();
	result->value = value <= other->value;
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_greater_or_equal(Type* other)
{
	Number* result = new Number();
	result->value = value >= other->value;
	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_and(Type* other)
{
	Number* result = new Number();

	if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = std::get<float>(value) && std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = (float)std::get<int>(value) && std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = std::get<float>(value) && (float)std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)std::get<int>(value) && std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 2 && other->value.index() == 2) {
		try { result->value = (bool)std::get<bool>(value) && std::get<bool>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_or(Type* other)
{
	Number* result = new Number();

	if (value.index() == 0 && other->value.index() == 0) {
		try { result->value = std::get<float>(value) || std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 1) {
		try { result->value = (float)std::get<int>(value) || std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 0 && other->value.index() == 1) {
		try { result->value = std::get<float>(value) || (float)std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1 && other->value.index() == 0) {
		try { result->value = (float)std::get<int>(value) ||  std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 2 && other->value.index() == 2) {
		try { result->value = (bool)std::get<bool>(value) || std::get<bool>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::compare_not(Type* other)
{
	Number* result = new Number();

	if (value.index() == 0) {
		try { result->value = std::get<float>(value) == 0.0f ? 1 : 0; }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1) {
		try { result->value = std::get<int>(value) == 0 ? 1 : 0; }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Number*, Error*> Number::is_true()
{
	Number* result = new Number();

	if (value.index() == 0) {
		try { result->value = std::get<float>(value) != 0.0f; }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 1) {
		try { result->value = std::get<int>(value) != 0; }
		catch (const std::bad_variant_access&) {}
	}
	else if (value.index() == 2) {
		try { result->value = std::get<bool>(value) != false; }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}
