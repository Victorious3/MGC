#pragma once
#include <stdafx.h>

namespace ini {
	class IniSection;
	class IniKey final {
		friend IniSection;

	private:
		IniKey(IniSection* parent, string key_name) : parent(parent), name(key_name) {}
		IniKey(IniSection* parent, string key_name, string key_value) : parent(parent), name(key_name), value(key_value) {}

	public:
		string value;
		string name;

		vector<string> comments;
		IniSection* parent;
	};

	bool operator==(const IniKey& k1, const IniKey& k2);
}
