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

Cursor Cursor::copy()
{
	return Cursor(index, line, column, filename, input);
}
