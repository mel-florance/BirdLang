#include "pch.h"
#include "Context.h"

Context::Context(
	const std::string& display_name,
	Context* parent,
	std::shared_ptr<Cursor> parent_cursor
) :
	display_name(display_name),
	parent(parent),
	parent_cursor(parent_cursor),
	symbols(nullptr)
{
}

std::ostream& operator<<(std::ostream& stream, Context* context)
{


	return stream;
}
