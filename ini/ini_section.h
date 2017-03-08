#pragma once

#include "../stddef.h"
#include "ini_file.h"
#include "ini_key.h"

#define ini_key($x, $def) $x = ini::current_section->get<decltype($x)>(#$x, ($def))

namespace ini {
	extern thread_local IniSection* current_section;

	class IniSection {
		friend IniFile;

	private:
		IniSection(IniFile* parent, string section_name) : parent(parent), name(section_name) {};

		IniFile* parent;
		string name;

		list<IniKey> keys;

		vector<string> comments{};

	public:
		bool rename(string new_name);

		IniKey* add_key(string key_name);

		bool remove_key(IniKey* key);
		bool remove_key(string key_name);

		bool rename_key(IniKey* key, string new_name);
		bool rename_key(string old_name, string new_name);

		const IniKey* get_key(string key_name) const;
		IniKey* get_key(string key_name);
		string get_key_value(string key_name) const;
		bool set_key_value(string key_name, string key_value);

		template<typename T>
		T get(string key_name, T default);

		template<typename R>
		R read_all() {
			current_section = this;
			R r;
			current_section = nullptr; // Don't keep the reference valid, would yield surprising behavior when used wrong
			return r;
		}

		string get_name() const;

		IniFile* get_parent() const;

		vector<string> get_key_names() const;

		void set_comments(vector<string> comments);
		vector<string> get_comments() const;
	};

	bool operator==(const IniSection& s1, const IniSection& s2);
}
