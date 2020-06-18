#include "Number.h"

Number::Number(const std::variant<float, int>& value) :
    value(value),
    start(nullptr),
    end(nullptr) {

}

Number* Number::add(Number* other)
{
    Number* result = new Number();
    
    if (value.index() == 0) {
		try { result->value = std::get<float>(value) + std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
    }
    else {
		try { result->value = std::get<int>(value) + std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
    }

    return result;
}

Number* Number::subtract(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0) {
		try { result->value = std::get<float>(value) - std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result->value = std::get<int>(value) - std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}

Number* Number::multiply(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0) {
		try { result->value = std::get<float>(value) * std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result->value = std::get<int>(value) * std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}

Number* Number::divide(Number* other)
{
	Number* result = new Number();

	if (value.index() == 0) {
		try { result->value = std::get<float>(value) / std::get<float>(other->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result->value = std::get<int>(value) / std::get<int>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}
