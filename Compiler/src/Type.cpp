#include "pch.h"
#include "Type.h"
#include "Number.h"
#include "Function.h"
#include "Str.h"
#include "Array.h"
#include "Interpreter.h"

Type::Type(
	const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value,
	std::shared_ptr<Cursor> start,
	std::shared_ptr<Cursor> end,
	Context* context
)
{
	this->value = value;
	this->start = start;
	this->end = end;
	this->context = context;
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

std::ostream& operator << (std::ostream& stream, Type* type)
{
	if (type != nullptr)
	{
		if (Array* array = dynamic_cast<Array*>(type)) {
			try {
				auto elements = std::get<std::vector<Type*>>(array->value);
				std::vector<Type*>::iterator it = elements.begin();

				for (; it != elements.end(); ++it) {
					stream << *it << '\n';
				}
			}
			catch (const std::bad_variant_access&) {}
		}
		else if (Number* number = dynamic_cast<Number*>(type)) {
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

			return stream;
		}
		else if (Function* function = dynamic_cast<Function*>(type)) {
			return stream << "<function " << function->name << ">";
		}
		else if (String* string = dynamic_cast<String*>(type)) {
			try { stream << std::get<std::string>(string->value); }
			catch (const std::bad_variant_access&) {}
		}
	}
	
	return stream;
}
