#include "stddef.h"
#include "filesystem.h"

#include <cstdlib>

namespace fs {
	Uint64 file_size(const string& file) {
		ifstream in(file, ifstream::ate | ifstream::binary);
		return in.tellg();
	}

#ifdef WIN32
	vector<string> list_files(const string& dir) {
		vector<string> result;

		HANDLE handle;
		WIN32_FIND_DATAW data;
		
		std::wstring dirw = dir.empty() ? L"*" : (utf8_decode(dir) + L"/*");
		handle = FindFirstFileW(dirw.c_str(), &data);
		if (handle != INVALID_HANDLE_VALUE) {
			if (!dir.empty()) {
				// Skip . and .. respectively
				// If those arent the first two files then hell breaks lose.
				FindNextFileW(handle, &data);
				FindNextFileW(handle, &data);
			}

			do {
				result.push_back(utf8_encode(data.cFileName));
			} while (FindNextFileW(handle, &data));
			FindClose(handle);
		}
		else throw runtime_error("Invalid handle, file \"" + dir + "\"");
		return result;
	}
#else
#error "LINUX STUB"
#endif

	vector<string> list_files(const string& dir, function<bool(const string&)> filter) {
		vector<string> result;
		vector<string> all_files = list_files(dir);
		for (string str : all_files) {
			if (filter(str)) result.push_back(str);
		}
		return result;
	}
}