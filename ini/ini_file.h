#pragma once
#include "../stddef.h"
#include "ini_record.h"

namespace ini {
	class IniSection;

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
		IniSection* const get_section(string section_name);
		const IniSection* const get_section(string section_name) const;

		string get_key_value(string section_name, string key_name) const;
		bool set_key_value(string section_name, string key_name, string key_value);

		string get_path() const;

		template<typename R>
		R read_all(const string& section) {
			return get_section(section)->read_all<R>();
		}

	private:
		string path;

		list<IniSection> sections;
	};
}
