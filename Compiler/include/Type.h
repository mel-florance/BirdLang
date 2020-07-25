#pragma once

#include "Context.h"

class Number;
class Function;
class Error;
class RuntimeResult;

class Type {
public:
	Type(
		const std::variant<double, int, bool, Function*>& value = 0,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	friend std::ostream& operator << (std::ostream& stream, Type* type);
	virtual RuntimeResult* execute(const std::vector<Type*>& args, Context* context);

	virtual std::pair<Number*, Error*> add(Type* other);
	virtual std::pair<Number*, Error*> subtract(Type* other);
	virtual std::pair<Number*, Error*> multiply(Type* other);
	virtual std::pair<Number*, Error*> modulus(Type* other);
	virtual std::pair<Number*, Error*> divide(Type* other);
	virtual std::pair<Number*, Error*> power(Type* other);
	virtual std::pair<Number*, Error*> compare_equal(Type* other);
	virtual std::pair<Number*, Error*> compare_not_equal(Type* other);
	virtual std::pair<Number*, Error*> compare_less_than(Type* other);
	virtual std::pair<Number*, Error*> compare_greater_than(Type* other);
	virtual std::pair<Number*, Error*> compare_less_or_equal(Type* other);
	virtual std::pair<Number*, Error*> compare_greater_or_equal(Type* other);
	virtual std::pair<Number*, Error*> compare_and(Type* other);
	virtual std::pair<Number*, Error*> compare_or(Type* other);
	virtual std::pair<Number*, Error*> compare_not(Type* other);
	virtual std::pair<Number*, Error*> is_true();

	std::variant<double, int, bool, Function*> value;
	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	Context* context;
};
