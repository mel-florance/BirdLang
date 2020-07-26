#pragma once

#include "Cursor.h"
#include "Error.h"
#include "Context.h"
#include "Platform.h"
#include "Type.h"

class Function;
class Number : public Type {
public:
	Number(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value = 0);

	std::pair<Type*, Error*> add(Type* other) override;
	std::pair<Type*, Error*> subtract(Type* other) override;
	std::pair<Type*, Error*> multiply(Type* other) override;
	std::pair<Type*, Error*> modulus(Type* other) override;
	std::pair<Type*, Error*> divide(Type* other) override;
	std::pair<Type*, Error*> power(Type* other) override;
	std::pair<Type*, Error*> compare_equal(Type* other) override;
	std::pair<Type*, Error*> compare_not_equal(Type* other) override;
	std::pair<Type*, Error*> compare_less_than(Type* other) override;
	std::pair<Type*, Error*> compare_greater_than(Type* other) override;
	std::pair<Type*, Error*> compare_less_or_equal(Type* other) override;
	std::pair<Type*, Error*> compare_greater_or_equal(Type* other) override;
	std::pair<Type*, Error*> compare_and(Type* other) override;
	std::pair<Type*, Error*> compare_or(Type* other) override;
	std::pair<Type*, Error*> compare_not(Type* other) override;
	std::pair<Type*, Error*> is_true();
};
