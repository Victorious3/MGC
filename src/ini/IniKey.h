#pragma once
#include <stdafx.h>

namespace ini {
	class IniSection;
	class IniKey final {

	private:
		friend IniSection;

		IniKey(IniSection* const parent, string key_name) : parent(parent), name(key_name) {}
		IniKey(IniSection* const parent, string key_name, string key_value) : parent(parent), name(key_name), value(key_value) {}

		string name;

	public:
		string value;
		vector<string> comments;
		IniSection* const parent;

		string get_name() const { return name; }
		bool rename(string new_name);
	};

	bool operator==(const IniKey& k1, const IniKey& k2);
}
