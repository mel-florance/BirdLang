#pragma once

#include "Type.h"

class File : public Type {
public:
	enum Mode {
		READ = 0x01,
		WRITE = 0x02,
		READ_WRITE = 0x01 | 0x02,
		END = 0x04,
		APPEND = 0x08,
		TRUNCATE = 0x10,
		BINARY = 0x20
	};

	File(
		const DynamicType& value = 0,
		const std::string& name = "",
		Mode mode = Mode::READ
	);

	inline static std::string modeToStr(Mode mode) {
		switch (mode) {
		default:
		case Mode::READ: return "r";
		case Mode::WRITE: return "w";
		case Mode::READ_WRITE: return "rw";
		case Mode::END: return "e";
		case Mode::APPEND: return "a";
		case Mode::TRUNCATE: return "t";
		case Mode::BINARY: return "b";
		}
	}

	int size;
	std::string name;
	int mode;
	bool closed;
};
