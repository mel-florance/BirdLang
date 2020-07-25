#pragma once

#include "Cursor.h"
#include "Error.h"
#include "Context.h"
#include "Platform.h"
#include "Type.h"

class Function;
class Number : public Type {
public:
	Number(const std::variant<float, int, bool, Function*>& value = 0);

	std::pair<Number*, Error*> add(Type* other) override;
	std::pair<Number*, Error*> subtract(Type* other) override;
	std::pair<Number*, Error*> multiply(Type* other) override;
	std::pair<Number*, Error*> modulus(Type* other) override;
	std::pair<Number*, Error*> divide(Type* other) override;
	std::pair<Number*, Error*> power(Type* other) override;
	std::pair<Number*, Error*> compare_equal(Type* other) override;
	std::pair<Number*, Error*> compare_not_equal(Type* other) override;
	std::pair<Number*, Error*> compare_less_than(Type* other) override;
	std::pair<Number*, Error*> compare_greater_than(Type* other) override;
	std::pair<Number*, Error*> compare_less_or_equal(Type* other) override;
	std::pair<Number*, Error*> compare_greater_or_equal(Type* other) override;
	std::pair<Number*, Error*> compare_and(Type* other) override;
	std::pair<Number*, Error*> compare_or(Type* other) override;
	std::pair<Number*, Error*> compare_not(Type* other) override;
	std::pair<Number*, Error*> is_true();
};
