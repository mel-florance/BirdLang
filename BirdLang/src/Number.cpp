#include "Number.h"

Number::Number(const std::variant<float, int>& value) :
    value(value),
    start(nullptr),
    end(nullptr) {

}

Number Number::add(const Number& other)
{
    Number result;
    
    if (value.index() == 0) {
		try { result.value = std::get<float>(value) + std::get<float>(other.value); }
		catch (const std::bad_variant_access&) {}
    }
    else {
		try { result.value = std::get<int>(value) + std::get<int>(other.value); }
		catch (const std::bad_variant_access&) {}
    }

    return result;
}

Number Number::subtract(const Number& other)
{
	Number result;

	if (value.index() == 0) {
		try { result.value = std::get<float>(value) - std::get<float>(other.value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result.value = std::get<int>(value) - std::get<int>(other.value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}

Number Number::multiply(const Number& other)
{
	Number result;

	if (value.index() == 0) {
		try { result.value = std::get<float>(value) * std::get<float>(other.value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result.value = std::get<int>(value) * std::get<int>(other.value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}

Number Number::divide(const Number& other)
{
	Number result;

	if (value.index() == 0) {
		try { result.value = std::get<float>(value) / std::get<float>(other.value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { result.value = std::get<int>(value) / std::get<int>(other.value); }
		catch (const std::bad_variant_access&) {}
	}

	return result;
}
