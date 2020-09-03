#include "pch.h"
#include "File.h"

File::File(
	const DynamicType& value,
	const std::string& name,
	Mode mode
) :
	Type(value)
{
	this->name = name;
	this->size = 0;
	this->mode = mode;
	this->closed = true;
}
