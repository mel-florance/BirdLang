#pragma once
#include <map>
#include "Context.h"

class Number;
class Function;
class Array;
class String;
class File;
class Map;
class Object;

class Error;
class RuntimeResult;

using DynamicType = std::variant<
	double,
	int,
	bool,
	Function*,
	std::string,
	std::vector<Type*>,
	File*,
	std::map<std::string, Type*>
>;

class Type {
public:
	enum Native {
		DOUBLE,
		INT,
		BOOL,
		FUNCTION,
		STRING,
		ARRAY,
		FILE,
		MAP,
		OBJECT
	};

	Type(
		const DynamicType& value = 0,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	friend std::ostream& operator << (std::ostream& stream, Type* type);
	virtual RuntimeResult* execute(const std::vector<Type*>& args, Context* context);
	inline bool is(Native type) { return value.index() == type; }

	static void printArray(std::ostream& stream, Array* array);
	static void printFunction(std::ostream& stream, Function* function);
	static void printString(std::ostream& stream, String* string);
	static void printNumber(std::ostream& stream, Number* number);
	static void printFile(std::ostream& stream, File* file);
	static void printMap(std::ostream& stream, Map* map);
	static void printObject(std::ostream& stream, Object* map);

	virtual std::pair<Type*, Error*> add(Type* other);
	virtual std::pair<Type*, Error*> subtract(Type* other);
	virtual std::pair<Type*, Error*> multiply(Type* other);
	virtual std::pair<Type*, Error*> modulus(Type* other);
	virtual std::pair<Type*, Error*> divide(Type* other);
	virtual std::pair<Type*, Error*> power(Type* other);
	virtual std::pair<Type*, Error*> compare_equal(Type* other);
	virtual std::pair<Type*, Error*> compare_not_equal(Type* other);
	virtual std::pair<Type*, Error*> compare_less_than(Type* other);
	virtual std::pair<Type*, Error*> compare_greater_than(Type* other);
	virtual std::pair<Type*, Error*> compare_less_or_equal(Type* other);
	virtual std::pair<Type*, Error*> compare_greater_or_equal(Type* other);
	virtual std::pair<Type*, Error*> compare_and(Type* other);
	virtual std::pair<Type*, Error*> compare_or(Type* other);
	virtual std::pair<Type*, Error*> compare_not(Type* other);
	virtual std::pair<Type*, Error*> is_true();

	static bool Null;
	static bool False;
	static bool True;

	DynamicType value;
	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	Context* context;
	int depth;
};
