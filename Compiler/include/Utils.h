#pragma once

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

class Utils {
public:
	inline static void title(const std::string& title, int size = 32, bool drawLastLine = true) {

		auto width = size / 2 + title.size() / 2;

		std::cout << '+' << std::string(size + title.size() * 2, '-') << "+\n";
		std::cout << "|" << std::string(width, ' ') << title << std::string(width, ' ') << "|\n";

		if (drawLastLine)
			std::cout << '+' << std::string(size + title.size() * 2, '-') << "+\n";
	}

	template<typename T>
	inline static T getVariantValue(const std::variant<float, int, bool>& variant) {
		T result = T();

		try { result = std::get<T>(variant); }
		catch (const std::bad_variant_access&) {}

		return result;
	}

	static inline std::vector<std::string> splitString(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> strings;
		std::string::size_type pos = 0;
		std::string::size_type prev = 0;

		while ((pos = str.find(delimiter, prev)) != std::string::npos)
		{
			strings.push_back(str.substr(prev, pos - prev));
			prev = pos + 1;
		}

		strings.push_back(str.substr(prev));

		return strings;
	}

	static inline std::string bytesToSize(int size)
	{
		const char* sizes[5] = { "bytes", "Kb", "Mb", "Gb", "Tb" };

		int i;
		double dblByte = size;

		for (i = 0; i < 5 && size >= 1024; i++, size /= 1024)
			dblByte = size / 1024.0;

		return std::to_string(dblByte) + " " + std::string(sizes[i]);
	}

	static inline float mapValues(float rangeA[], float rangeB[], float value, unsigned int size = 2)
	{
		return (value - rangeA[0]) * (rangeB[1] - rangeB[0]) / (rangeA[1] - rangeA[0]) + rangeB[0];
	}


	static inline std::string& ltrim(std::string& str)
	{
		size_t start = str.find_first_not_of(" \t\r\n\v\f");

		if (std::string::npos != start)
			str = str.substr(start);

		return str;
	}

	static inline std::string& rtrim(std::string& str)
	{
		size_t end = str.find_last_not_of(" \t\r\n\v\f");

		if (std::string::npos != end)
			str = str.substr(0, end + 1);

		return str;
	}
};
