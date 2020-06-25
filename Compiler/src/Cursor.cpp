#include "pch.h"
#include "Cursor.h"

void Cursor::advance(const char& character)
{
	++index;
	++column;

	if (character == '\n') {
		++line;
		column = 0;
	}
}