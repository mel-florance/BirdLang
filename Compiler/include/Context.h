#pragma once

#include <memory>

#include "Cursor.h"
#include "Symbols.h"
#include "Platform.h"

class Context {
public:
	Context(
		const std::string& display_name,
		Context* parent = nullptr,
		std::shared_ptr<Cursor> parent_cursor = nullptr
	);

	friend std::ostream& operator << (std::ostream& stream, Context* context);

	std::string display_name;
	Context* parent;
	std::shared_ptr<Cursor> parent_cursor;
	Symbols* symbols;
};
