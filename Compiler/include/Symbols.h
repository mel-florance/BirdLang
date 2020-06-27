#pragma once

#include "Platform.h"

#include <unordered_map>

class API Symbols {
public:
	Symbols();

	typedef std::unordered_map<std::string, std::variant<float, int>> SymbolsMap;

	SymbolsMap::iterator get(const std::string& name);
	void set(const std::string& name, std::variant<float, int> value);
	bool remove(const std::string& name);
	
	SymbolsMap symbols;
	Symbols* parent;
};