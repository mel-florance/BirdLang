#pragma once

class Cursor
{
public:
	Cursor(size_t index = -1, int line = 0, int column = -1, const std::string& filename = "", const std::string& input = "") :
		index(index),
		line(line),
		column(column),
		filename(filename),
		input(input) {}

	Cursor(const Cursor& cursor) {
		index = cursor.index;
		line = cursor.line;
		column = cursor.column;
		filename = cursor.filename;
		input = cursor.input;
	}

	Cursor(Cursor* cursor) {
		if (cursor != nullptr) {
			index = cursor->index;
			line = cursor->line;
			column = cursor->column;
			filename = cursor->filename;
			input = cursor->input;
		}
	}

	size_t index;
	int line;
	int column;
	std::string filename;
	std::string input;

	void advance(const char& character = NULL);
};

