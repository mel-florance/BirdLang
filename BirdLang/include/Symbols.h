#pragma once

#include <unordered_map>
#include <variant>
#include <iostream>

class Symbols {
public:
	Symbols();

	typedef std::unordered_map<std::string, std::variant<float, int>> SymbolsMap;

	SymbolsMap::iterator get(const std::string& name);
	void set(const std::string& name, std::variant<float, int> value);
	bool remove(const std::string& name);
	
	SymbolsMap symbols;
	Symbols* parent;
};