#pragma once

#include <string>
using std::string;

static inline string string_trim(string& input) {
	size_t first = input.find_first_not_of(' ');
	size_t last = input.find_last_not_of(' ');
	return input.substr(first, (last - first + 1));
}

static inline string string_tolower(string& input) {
	auto output = string(input);
	std::locale loc;

	for (auto& elem : output) {
		elem = std::tolower(elem, loc);
	}
	return output;
}