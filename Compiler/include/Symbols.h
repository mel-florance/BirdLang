#pragma once

#include "Platform.h"

#include <unordered_map>

class Function;
class Symbols {
public:
	Symbols(Symbols* parent = nullptr);

	typedef std::unordered_map<std::string, std::variant<double, int, bool, Function*>> SymbolsMap;

	SymbolsMap::iterator get(const std::string& name);
<<<<<<< Updated upstream
	void set(const std::string& name, std::variant<float, int, bool, Function*> value);
=======
	void set(const std::string& name, std::variant<double, int, bool, Function*> value);

>>>>>>> Stashed changes
	bool remove(const std::string& name);
	
	SymbolsMap symbols;
	Symbols* parent;
};
