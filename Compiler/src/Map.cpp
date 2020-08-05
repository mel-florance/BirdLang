#include "pch.h"
#include "Map.h"

Map::Map(const DynamicType& value) :
	Type(value)
{
}

std::pair<Type*, Error*> Map::compare_less_than(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Map::subtract(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Map::add(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Map::compare_greater_than(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Map::modulus(Type* other)
{
	return std::pair<Type*, Error*>();
}
