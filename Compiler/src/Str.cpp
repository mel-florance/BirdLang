#include "pch.h"
#include "Str.h"
#include "Number.h"

const std::string str_ascii_lowercase = "abcdefghijklmnopqrstuvwxyz";
const std::string str_ascii_uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string str_digits = "0123456789";
const std::string str_hexdigits = "0123456789abcdefABCDEF";
const std::string str_octdigits = "01234567";
const std::string str_punctuation = "!\"#$ % &'()*+,-./:;<=>?@[\\]^_`{|}~";
const std::string str_vowels = "aeiouy";

std::unordered_map<std::string, std::string> String::constants = {
	{"ascii_lowercase", str_ascii_lowercase},
	{"ascii_uppercase", str_ascii_uppercase},
	{"ascii_letters", str_ascii_lowercase + str_ascii_uppercase},
	{"digits", str_digits},
	{"hexdigits", str_hexdigits},
	{"octdigits", str_octdigits},
	{"punctuation", str_punctuation},
	{"printable", str_digits + str_ascii_lowercase + str_ascii_uppercase + str_punctuation},
	{"vowels", str_vowels}
};

String::String(const DynamicType& value) :
	Type(value)
{
}

std::pair<Type*, Error*> String::add(Type* other)
{
	auto str = new String();

	if(other->value.index() == Type::Native::DOUBLE) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<double>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == Type::Native::INT) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<int>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == Type::Native::BOOL) {
		try { str->value = std::get<std::string>(value) + std::to_string(std::get<bool>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	if (other->value.index() == Type::Native::STRING) {
		try { str->value = std::get<std::string>(value) + std::get<std::string>(other->value); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(str, nullptr);
}

std::pair<Type*, Error*> String::multiply(Type* other)
{
	auto str = new String();

	if (other->value.index() == Type::Native::INT) {
		std::string out;
		auto n = std::get<int>(other->value);

		while (n--) {
			out += std::get<std::string>(value);
		}

		str->value = out;
	}

	return std::make_pair(str, nullptr);
}

std::pair<Type*, Error*> String::compare_equal(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value) == std::get<std::string>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Type*, Error*> String::compare_not_equal(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value) != std::get<std::string>(other->value)); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Type*, Error*> String::compare_less_than(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value).size() < std::get<std::string>(other->value).size()); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Type*, Error*> String::compare_greater_than(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value).size() > std::get<std::string>(other->value).size()); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Type*, Error*> String::compare_less_or_equal(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value).size() <= std::get<std::string>(other->value).size()); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}

std::pair<Type*, Error*> String::compare_greater_or_equal(Type* other)
{
	auto result = new Number(false);

	if (other->value.index() == Type::Native::STRING) {
		try { result->value = (bool)(std::get<std::string>(value).size() >= std::get<std::string>(other->value).size()); }
		catch (const std::bad_variant_access&) {}
	}

	return std::make_pair(result, nullptr);
}
