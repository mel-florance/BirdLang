#include "pch.h"
#include "Array.h"
#include "Number.h"
#include "Str.h"

Array::Array(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value) :
	Type(value)
{
}

std::pair<Type*, Error*> Array::add(Type* other)
{
	auto array = std::get<std::vector<Type*>>(value);
	array.push_back(other);
	value = std::move(array);

	return std::make_pair(this, nullptr);
}
