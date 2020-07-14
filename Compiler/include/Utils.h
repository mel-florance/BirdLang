#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

class Utils {
public:
	inline static void title(const std::string& title, int size = 32) {

		auto width = size / 2 + title.size() / 2;

		std::cout << '+' << std::string(size + title.size() * 2, '-') << "+\n";
		std::cout << "|" << std::string(width, ' ') << title << std::string(width, ' ') << "|\n";
		std::cout << '+' << std::string(size + title.size() * 2, '-') << "+\n";
	}
};
