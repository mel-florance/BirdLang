#pragma once

#include "Platform.h"

#include <unordered_map>

class Symbols {
public:
	Symbols();

	typedef std::unordered_map<std::string, std::variant<double, int, bool>> SymbolsMap;

	SymbolsMap::iterator get(const std::string& name);
	void set(const std::string& name, std::variant<double, int, bool> value);
	bool remove(const std::string& name);
	
	SymbolsMap symbols;
	Symbols* parent;
};
