#include "pch.h"
#include "Str.h"

String::String(const std::variant<double, int, bool, Function*, std::string>& value) :
	Type(value)
{
}

std::pair<Type*, Error*> String::add(Type* other)
{
	auto str = new String();

	if (other->value.index() == 4) {
		try { str->value = std::get<std::string>(value) + std::get<std::string>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(str, nullptr);
}

std::pair<Type*, Error*> String::multiply(Type* other)
{
	return std::pair<Type*, Error*>();
}
