#pragma once

#include "stddef.h"

#include <map>
using std::map;

namespace mgc {
	class Ini_Section;

	class Ini_File {
	public:
		Ini_File();
		~Ini_File();

		Ini_File(string path);

		bool load_file(string path);
		bool save_file();
		bool save_file(string path);

		Ini_Section* add_section(string section_name);
		bool remove_section(string section_name);
		bool remove_section(Ini_Section* section);

		bool rename_section(string old_name, string new_name);
		bool rename_section(Ini_Section* section, string new_name);

		vector<string> get_section_names();
		Ini_Section* get_section(string section_name);

		string get_key_value(string section_name, string key_name);
		bool set_key_value(string section_name, string key_name, string key_value);

		string get_path();

	private:
		string path;

		list<Ini_Section> sections;
	};
}