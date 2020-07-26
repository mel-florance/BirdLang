#include "pch.h"
#include "Array.h"
#include "Number.h"
#include "Str.h"

Array::Array(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value) :
	Type(value)
{
}

std::pair<Type*, Error*> Array::compare_less_than(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);
	array.push_back(other);
	value = std::move(array);

	return std::make_pair(this, nullptr);
}

std::pair<Type*, Error*> Array::subtract(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);

	if (other->value.index() == 1 && array.size() > 0) {
		auto index = std::get<int>(other->value);

		if (index < array.size() && index >= 0) {
			array.erase(array.begin() + index);
			value = std::move(array);
		}
	}

	return std::make_pair(this, nullptr);
}

std::pair<Type*, Error*> Array::add(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);
	auto elements = std::get<std::vector<Type*>>(other->value);
	std::vector<Type*>::iterator it = elements.begin();

	for (; it != elements.end(); ++it)
		array.push_back(*it);
	
	value = std::move(array);

	return std::make_pair(this, nullptr);
}

std::pair<Type*, Error*> Array::compare_greater_than(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);

	if (other->value.index() == 1) {
		auto index = std::get<int>(other->value);

		if (index < array.size()) {
			auto element = array.at(index);
			return std::make_pair(element, nullptr);
		}
	}

	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Array::modulus(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);
	return std::make_pair(new Number((int)array.size()), nullptr);
}
