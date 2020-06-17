#pragma once

#include <string>

class Cursor
{
public:
	Cursor(size_t index = -1, int line = 0, int column = -1, const std::string& filename = "", const std::string& input = "") :
		index(index),
		line(line),
		column(column),
		filename(filename),
		input(input) {}

	size_t index;
	int line;
	int column;
	std::string filename;
	std::string input;

	void advance(const char& character);
	Cursor copy();
};

