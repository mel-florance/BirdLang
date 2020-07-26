#pragma once

#include "Type.h"

class Array : public Type {
public:
	Array(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value = 0);

	std::pair<Type*, Error*> compare_less_than(Type* other) override;
	std::pair<Type*, Error*> subtract(Type* other) override;
	std::pair<Type*, Error*> add(Type* other) override;
	std::pair<Type*, Error*> compare_greater_than(Type* other) override;
	std::pair<Type*, Error*> modulus(Type* other) override;
};
