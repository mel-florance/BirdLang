#pragma once

#include "Cursor.h"
#include "Context.h"

class Error
{
public:
	Error(const std::string& name, const Cursor& start, const Cursor& end, const std::string& details) : 
		name(name),
		start(start),
		end(end),
		details(details) {}

	virtual ~Error() {}

	inline friend std::ostream& operator << (std::ostream& stream, Error* error) {
		return stream << "\n\x1B[31m" <<
			error->name << ": " << error->details << "\n" <<
			"File " << error->start.filename << ", line " << std::to_string(error->start.line + 1) <<
			"\033[0m\t\t";
	}
	
	std::string name;
	Cursor start;
	Cursor end;
	std::string details;
};


class IllegarCharError : public Error {
public:
	IllegarCharError(const Cursor& start, const Cursor& end, const std::string& details) :
		Error("Illegal Character", start, end, details) {}
};

class InvalidSyntaxError : public Error {
public:
	InvalidSyntaxError(const Cursor& start, const Cursor& end, const std::string& details) : 
		Error("Invalid Syntax", start, end, details) {}
};

class RuntimeError : public Error {
public:
	RuntimeError(const Cursor& start, const Cursor& end, const std::string& details, Context* context) :
		Error("Runtime Error", start, end, details),
		context(context)
	{}

	inline std::string traceback() {
		std::string result;
		Cursor* pos = &start;
		Context* ctx = context;

		while (ctx != nullptr) {
			result = "  File " + start.filename + ", Line " + std::to_string(start.line + 1) + ", in " + ctx->display_name + "\n" + result;
			pos = ctx->parent_cursor;
			ctx = ctx->parent;
		}

		return "Traceback: (Most recent calls):\n" + result;
	}

	inline friend std::ostream& operator << (std::ostream& stream, RuntimeError* error) {
		return stream << "\n\x1B[31m" << 
			error->traceback() <<
			error->name << ": " << error->details << "\n" <<
			"\033[0m\t\t";
	}

	Context* context;
};
