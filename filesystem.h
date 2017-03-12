#pragma once
#include "stddef.h"

#include <fstream>
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::istream;

namespace fs {
	vector<string> list_files(const string& dir);
	vector<string> list_files(const string& dir, function<bool(const string&)> filter);

	Uint64 file_size(const string& file);

	void read_line(istream& is, string& t);
}