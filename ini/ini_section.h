#pragma once

#include "../stddef.h"

namespace ini {
	class IniFile;
	class IniKey;

	class IniSection {
		friend IniFile;

	private:
		IniSection(IniFile* parent, string section_name) : parent(parent), name(section_name) {};

		IniFile* parent;
		string name;

		list<IniKey> keys;

	public:
		bool rename(string new_name);

		IniKey* add_key(string key_name);

		bool remove_key(IniKey* key);
		bool remove_key(string key_name);

		bool rename_key(IniKey* key, string new_name);
		bool rename_key(string old_name, string new_name);

		IniKey* get_key(string key_name);
		const IniKey* get_key(string key_name) const;
		string get_key_value(string key_name) const;
		bool set_key_value(string key_name, string key_value);

		string get_name() const;

		IniFile* get_parent() const;

		
	};

	bool operator==(const IniSection& s1, const IniSection& s2);
}