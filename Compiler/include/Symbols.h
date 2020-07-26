#pragma once

#include "Platform.h"

#include <unordered_map>

class Function;
class Type;
class Symbols {
public:
	Symbols(Symbols* parent = nullptr);

	typedef std::unordered_map<std::string, std::variant<double, int, bool, Function*, std::string, std::vector<Type*>>> SymbolsMap;

	SymbolsMap::iterator get(const std::string& name);
	void set(const std::string& name, std::variant<double, int, bool, Function*, std::string, std::vector<Type*>> value);
	bool remove(const std::string& name);
	
	SymbolsMap symbols;
	Symbols* parent;
};
