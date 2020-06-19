#pragma once

#include <string>

#include "Cursor.h"

class Context {
public:
	Context(const std::string& display_name, Context* parent = nullptr, Cursor* parent_cursor = nullptr);

	std::string display_name;
	Context* parent;
	Cursor* parent_cursor;
};