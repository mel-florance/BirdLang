#pragma once

#include "Type.h"

class String : public Type {
public:
	String(const std::variant<double, int, bool, Function*, std::string>& value = 0);

	std::pair<Type*, Error*> add(Type* other) override;
	std::pair<Type*, Error*> multiply(Type* other) override;
};
