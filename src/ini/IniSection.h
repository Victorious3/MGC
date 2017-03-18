#pragma once
#include <stdafx.h>

#include "IniFile.h"
#include "IniKey.h"

#define ini_key($x, $def) $x = ini::_current_section->get<decltype($x)>(#$x, ($def))

namespace ini {
	// Internal
	extern thread_local IniSection* _current_section;

	class IniSection {
	private:
		friend IniFile;
		friend void fill_ini_file(string path, IniFile& ini_file);

		IniSection(IniFile* parent, string section_name) : parent(parent), name(section_name) {};

		list<IniKey> keys;
		vector<string> comments;
		string name;

	public:

		IniFile* const parent;

		vector<string> get_comments() const { return comments; }
		string get_name() const { return name; }

		bool rename(const string new_name);

		// Adds a key if not present or else returns an existing one
		IniKey& add_key(const string key_name);

		bool remove_key(IniKey* key);
		bool remove_key(const string& key_name);

		bool rename_key(IniKey* key, const string new_name);
		bool rename_key(const string& old_name, const string new_name);

		const IniKey* get_key(const string& key_name) const;
		IniKey* get_key(const string& key_name);

		string& operator[](const string& key_name);

		template<typename T>
		T get(const string& key_name, T default) const;

		template<> 
		string get(const string& key_name, string def) const {
			if (const IniKey* key = get_key(key_name)) {
				return key->value;
			}
			return def;
		}

		template<typename T>
		void set(const string& key_name, T value) { 
			set<string>(key_name, std::to_string(value)); 
		}

		template<>
		void set(const string& key_name, string value) {
			add_key(key_name).value = value;
		}

		template<typename R>
		R read_all() {
			current_section = this;
			R r;
			current_section = nullptr; // Don't keep the reference valid, would yield surprising behavior when used wrong
			return r;
		}

		vector<string> get_key_names() const;
	};

	bool operator==(const IniSection& s1, const IniSection& s2);
}
