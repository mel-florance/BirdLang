#pragma once

#include <string>
#include "Cursor.h"

class Error
{
public:
	Error(const std::string& name, const Cursor& start, const Cursor& end, const std::string& details) : 
		name(name),
		start(start),
		end(end),
		details(details) {}

	inline friend std::ostream& operator << (std::ostream& stream, const Error& error) {
		return stream << "\n\x1B[31m" <<
			error.name << ": " << error.details << "\n" <<
			"File " << error.start.filename << ", line " << std::to_string(error.start.line + 1) << "\033[0m\t\t";
	}
	
	std::string name;
	Cursor start;
	Cursor end;
	std::string details;
};


class IllegarCharError : public Error {
public:
	IllegarCharError(const Cursor& start, const Cursor& end, const std::string& details) : Error("Illegal Character", start, end, details) {}
};
