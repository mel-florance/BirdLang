#include "pch.h"
#include "Object.h"

Object::Object(const DynamicType& value) :
	Type(value)
{
}

std::pair<Type*, Error*> Object::compare_less_than(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Object::subtract(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Object::add(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Object::compare_greater_than(Type* other)
{
	return std::pair<Type*, Error*>();
}

std::pair<Type*, Error*> Object::modulus(Type* other)
{
	return std::pair<Type*, Error*>();
}
