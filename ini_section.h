#pragma once

#include "stddef.h"

namespace mgc {
	class Ini_File;
	class Ini_Key;

	class Ini_Section {
		friend Ini_File;

	private:
		Ini_Section(Ini_File* parent, string section_name) : parent(parent), name(section_name) {};

		Ini_File* parent;
		string name;

		list<Ini_Key> keys;

	public:
		bool rename(string new_name);

		Ini_Key* add_key(string key_name);

		bool remove_key(Ini_Key* key);
		bool remove_key(string key_name);

		bool rename_key(Ini_Key* key, string new_name);
		bool rename_key(string old_name, string new_name);

		Ini_Key* get_key(string key_name);
		string get_key_value(string key_name);
		bool set_key_value(string key_name, string key_value);

		string get_name() const;

		Ini_File* get_parent() const;

		
	};

	bool operator==(const Ini_Section& s1, const Ini_Section& s2);
}