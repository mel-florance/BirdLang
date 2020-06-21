#include "pch.h"
#include "Symbols.h"

Symbols::Symbols() :
	symbols({}),
	parent(nullptr)
{

}

Symbols::SymbolsMap::iterator Symbols::get(const std::string& name)
{
	SymbolsMap::iterator it = symbols.find(name);

	if (it == symbols.end() && parent != nullptr)
		return parent->get(name);

	return it;
}

void Symbols::set(const std::string& name, std::variant<float, int> value)
{
	symbols[name] = value;
}

bool Symbols::remove(const std::string& name)
{
	return symbols.erase(name) == 1;
}

