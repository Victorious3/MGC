#pragma once
#include <stdafx.h>

namespace ini {
	class IniSection;
	class IniKey {
		friend IniSection;

	private:
		IniKey(IniSection* parent, string key_name) : parent(parent), name(key_name) {}
		IniKey(IniSection* parent, string key_name, string key_value) : parent(parent), name(key_name), value(key_value) {}

		IniSection* parent;

		string value;
		string name;
		vector<string> comments{};

	public:

		string get_value() const;
		void set_value(string value);

		bool rename(string new_name);

		string get_name() const;

		IniSection* get_parent() const;

		void set_comments(vector<string>);
		vector<string> get_comments() const;
	};

	bool operator==(const IniKey& k1, const IniKey& k2);
}
