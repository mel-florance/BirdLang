#pragma once

struct Url;
class Http {
public:
	static std::string get(const Url& url);
};
