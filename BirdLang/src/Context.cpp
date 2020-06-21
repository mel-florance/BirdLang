#include "pch.h"
#include "Context.h"

Context::Context(const std::string& display_name, Context* parent, Cursor* parent_cursor) :
	display_name(display_name),
	parent(parent),
	parent_cursor(parent_cursor),
	symbols(nullptr)
{
}
