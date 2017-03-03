#pragma once

#include "../stddef.h"

#include <map>
using std::map;

namespace ini {
	class IniFile;
	class IniSection;
	class IniKey;

	class IniFile {
	public:
		IniFile();
		~IniFile();

		IniFile(string path);

		bool load_file(string path);
		bool save_file();
		bool save_file(string path);

		IniSection* add_section(string section_name);
		bool remove_section(string section_name);
		bool remove_section(IniSection* section);

		bool rename_section(string old_name, string new_name);
		bool rename_section(IniSection* section, string new_name);

		vector<string> get_section_names() const;
		IniSection* get_section(string section_name);
		const IniSection* get_section(string section_name) const;

		string get_key_value(string section_name, string key_name) const;
		bool set_key_value(string section_name, string key_name, string key_value);

		string get_path() const;

	private:
		string path;

		list<IniSection> sections;
	};

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

		const IniKey* get_key(string key_name) const;
		IniKey* get_key(string key_name);
		string get_key_value(string key_name) const;
		bool set_key_value(string key_name, string key_value);

		string get_name() const;

		IniFile* get_parent() const;
	};

	class IniKey {
		friend IniSection;

	private:
		IniKey(IniSection* parent, string key_name) : parent(parent), name(key_name) {}
		IniKey(IniSection* parent, string key_name, string key_value) : parent(parent), name(key_name), value(key_value) {}

		IniSection* parent;

		string value;
		string name;

	public:

		string get_value() const;
		void set_value(string value);

		bool rename(string new_name);

		string get_name() const;

		IniSection* get_parent() const;
	};

	bool operator==(const IniSection& s1, const IniSection& s2);
	bool operator==(const IniKey& k1, const IniKey& k2);
}