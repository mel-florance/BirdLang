#include "pch.h"
#include "Str.h"

String::String(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value) :
	Type(value)
{
}

std::pair<Type*, Error*> String::add(Type* other)
{
	auto str = new String();

	if(other->value.index() == 0) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<double>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == 1) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<int>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == 2) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<bool>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == 4) {
		try { str->value = std::get<std::string>(value) + std::get<std::string>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(str, nullptr);
}

std::pair<Type*, Error*> String::multiply(Type* other)
{
	auto str = new String();

	if (other->value.index() == 1) {
		std::string out;
		auto n = std::get<int>(other->value);

		while (n--) {
			out += std::get<std::string>(value);
		}

		str->value = out;
	}

	return std::make_pair(str, nullptr);
}
