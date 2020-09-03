#include "pch.h"

#include "Type.h"
#include "Number.h"
#include "Function.h"
#include "Str.h"
#include "Array.h"
#include "File.h"

#include "Object.h"
#include "Interpreter.h"
#include "Map.h"
bool Type::Null = false;
bool Type::False = false;
bool Type::True = true;

Type::Type(
	const DynamicType& value,
	std::shared_ptr<Cursor> start,
	std::shared_ptr<Cursor> end,
	Context* context
)
{
	this->value = value;
	this->start = start;
	this->end = end;
	this->context = context;
	this->depth = 0;
}

RuntimeResult* Type::execute(const std::vector<Type*>& args, Context* context)
{
	return nullptr;
}

std::pair<Type*, Error*> Type::add(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::subtract(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::multiply(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::modulus(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::divide(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::power(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_equal(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_not_equal(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_less_than(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_greater_than(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_less_or_equal(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_greater_or_equal(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_and(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_or(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::compare_not(Type* other) {
	return std::pair<Number*, Error*>();
}

std::pair<Type*, Error*> Type::is_true() {
	return std::pair<Number*, Error*>();
}

void Type::printArray(std::ostream& stream, Array* array)
{
	auto elements = std::get<std::vector<Type*>>(array->value);
	std::vector<Type*>::iterator it = elements.begin();

	stream << std::string(array->depth, ' ') << "[" << '\n';
	array->depth += 4;

	for (unsigned int i = 0; it != elements.end(); ++it, i++) {
		auto comma = i != elements.size() - 1 ? ',' : '\0';
			
		stream << std::string(array->depth, ' ') <<
			*it << comma << '\n';
	}

	stream << std::string(array->depth, ' ') << "]" << '\n';
}

void Type::printFunction(std::ostream& stream, Function* function)
{
	stream << "<function " << function->name << ">";
}

void Type::printString(std::ostream& stream, String* string)
{
	try { stream << '"' << std::get<std::string>(string->value) << '"'; }
	catch (const std::bad_variant_access&) {}
}

void Type::printNumber(std::ostream& stream, Number* number)
{
	if (number->value.index() == 0) {
		try { stream << std::to_string(std::get<double>(number->value)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (number->value.index() == 1) {
		try { stream << std::to_string(std::get<int>(number->value)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (number->value.index() == 2) {
		try { stream << (std::get<bool>(number->value) == true ? "true" : "false"); }
		catch (const std::bad_variant_access&) {}
	}
}

void Type::printFile(std::ostream& stream, File* file)
{
	if (file != nullptr)
		stream << "<file " << std::filesystem::path(file->name).filename() << ">";
}

void Type::printMap(std::ostream& stream, Map* map)
{
	auto elements = std::get<std::map<std::string, Type*>>(map->value);
	std::map<std::string, Type*>::reverse_iterator it = elements.rbegin();

	stream << std::string(map->depth, ' ') << "{" << '\n';
	map->depth += 4;

	for (unsigned int i = 0; it != elements.rend(); ++it, i++) {
		auto comma = i != elements.size() - 1 ? ',' : '\0';

		stream << std::string(map->depth + 4, ' ') <<
			it->first << ": " << it->second << comma << '\n';
	}

	stream << std::string(map->depth, ' ') << "}";
	map->depth = 0;
}

void Type::printObject(std::ostream& stream, Object* map)
{
}

std::ostream& operator << (std::ostream& stream, Type* type)
{
	if (type != nullptr)
	{
		if (Array* array = dynamic_cast<Array*>(type)) {
			type->depth = 0;
			Type::printArray(stream, array);
		}
		else if (Map* map = dynamic_cast<Map*>(type)) {
			type->depth = 0;
			Type::printMap(stream, map);
		}
		else if (Number* number = dynamic_cast<Number*>(type))
			Type::printNumber(stream, number);
		else if (Function* function = dynamic_cast<Function*>(type))
			Type::printFunction(stream, function);
		else if (String* string = dynamic_cast<String*>(type))
			Type::printString(stream, string);
		else if (File* file = (File*)type)
			Type::printFile(stream, std::get<File*>(file->value));
	/*	else if (Object* object = (Object*)type)
			Type::printObject(stream, std::get<Object*>(object->value));*/
	}
	
	return stream;
}
