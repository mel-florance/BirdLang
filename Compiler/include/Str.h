#pragma once

#include "Type.h"

class String : public Type {
public:
	String(const std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>& value = 0);

	std::pair<Type*, Error*> add(Type* other) override;
	std::pair<Type*, Error*> multiply(Type* other) override;
	std::pair<Type*, Error*> compare_equal(Type* other) override;
	std::pair<Type*, Error*> compare_not_equal(Type* other) override;
	std::pair<Type*, Error*> compare_less_than(Type* other) override;
	std::pair<Type*, Error*> compare_greater_than(Type* other) override;
	std::pair<Type*, Error*> compare_less_or_equal(Type* other) override;
	std::pair<Type*, Error*> compare_greater_or_equal(Type* other) override;

	static std::unordered_map<std::string, std::string> constants;
};
