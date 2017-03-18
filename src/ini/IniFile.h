#pragma once
#include <stdafx.h>

namespace ini {
	class IniSection;

	class IniFile {
	public:
		~IniFile();

		IniFile(string path);

		//bool load_file(string path);
		void save_file() const;
		void save_file(string path) const;

		IniSection& add_section(string section_name);
		bool remove_section(const string& section_name);
		bool remove_section(IniSection& section);

		bool rename_section(const string& old_name, string new_name);
		bool rename_section(IniSection* section, string new_name);

		vector<string> get_section_names() const;
		IniSection* const get_section(string section_name);
		const IniSection* const get_section(string section_name) const;

		string get_key_value(string section_name, string key_name) const;
		bool set_key_value(string section_name, string key_name, string key_value);

		const string& path = path_;

		template<typename R>
		R read_all(const string& section) {
			return get_section(section)->read_all<R>();
		}

	private:
		friend void fill_ini_file(string path, IniFile& ini_file);
		friend IniFile load_ini_file(string path);

		IniFile();
		string path_;

		list<IniSection> sections;
	};
}
