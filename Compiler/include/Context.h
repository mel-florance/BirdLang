#pragma once

#include "Cursor.h"
#include "Symbols.h"
#include "Platform.h"

class API Context {
public:
	Context(const std::string& display_name, Context* parent = nullptr, Cursor* parent_cursor = nullptr);

	std::string display_name;
	Context* parent;
	Cursor* parent_cursor;
	Symbols* symbols;
};