#include "stddef.h"

#include <algorithm>

#include "../filesystem.h"
#include "ini_file.h"
#include "ini_section.h"
#include "ini_key.h"

using std::ifstream;

namespace ini {
	IniFile::IniFile() {
	}

	IniFile::IniFile(string path) {
		load_file(path);
	}

	IniFile::~IniFile() {
	}

	bool IniFile::load_file(string path) {
		this->path = path;
		ifstream file(path);

		if (!file) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not open ini file %s", path.c_str());
			return false;
		}

		int mode = 0;
		IniSection* current_cat = nullptr;
		char c;
		string buffer = "";
		IniKey* current_key = nullptr;
		int line_number = 0;

		while (file.get(c)) {
			if (c == '\r' || c == '\n') {
				if (c == '\r' && file.peek() == '\n') {
					file.get(c);
				}
				if (mode == 2) {
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Malformed ini file %s, new line while reading section header name. Line: %u", path, line_number);
					file.close();
					return false;
				}
				else if (mode == 3) {
					current_key->set_value(string_tolower(buffer));
					current_key = nullptr;
				}
				buffer = "";
				line_number++;
				mode = 0;
				continue;
			}

			if (mode == 0) {
				// Reading new line
				switch (c) {
				case ';':
					mode = 1;
					break;
				case ' ':
					buffer += c;
					break;
				case '[':
					buffer = "";
					mode = 2;
					continue;
					break;
				case '=':
					if (current_cat == nullptr) {
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Malformed ini file %s, key definition without category. Line: %u", path, line_number);
						file.close();
						return false;
					}
					current_key = current_cat->add_key(string_tolower(buffer));
					buffer = "";
					mode = 3;
					continue;
					break;
				default:
					buffer += c;
					continue;
				}
			}

			if (mode == 1) {
				// Comment line, ignore all
				continue;
			}
			else if (mode == 2) {
				// Section heading
				if (c == ']') {
					buffer = string_trim(buffer);
					if (get_section(buffer) != nullptr) {
						SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Malformed ini file %s, duplicate section header name %s. Line: %u", path, buffer, line_number);
						file.close();
						return false;
					}
					current_cat = add_section(string_tolower(buffer));
					buffer = "";
					mode = 1; // Ignore rest of line
				}
				else {
					buffer += c;
				}
			}
			else if (mode == 3) {
				buffer += c;
				//Reading key's value
				continue;
			}
		}

		return true;
	}

	IniSection* IniFile::add_section(string section_name) {
		sections.push_back(IniSection(this, section_name));
		return &sections.back();
	}

	bool IniFile::rename_section(string old_name, string new_name) {
		if (get_section(new_name) != nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to rename section %s to already existing section %s in file %s", old_name, new_name, path);
			return false;
		}

		for (auto& iter : sections) {
			if (iter.name == old_name) {
				iter.name = new_name;
				return true;
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to rename non-existant category %s to %s in file %s.", old_name, new_name, path);
		return false;
	}

	bool IniFile::rename_section(IniSection* section, string new_name) {
		if (section->parent != this) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to rename section %s in file %s to %s, using file %s", section->name, new_name, section->parent->get_path(), path);
			return false;
		}
		return rename_section(section->name, new_name);
	}

	const IniSection* const IniFile::get_section(string section_name) const {
		for (auto& iter : sections) {
			if (iter.get_name() == section_name) {
				return &iter;
			}
		}

		return nullptr;
	}

	IniSection* const IniFile::get_section(string section_name) {
		return const_cast<IniSection*> (
				static_cast<const IniFile*>(this)->get_section(section_name)
			);
	}



	bool IniFile::remove_section(string section_name) {
		for (auto& iter : sections) {
			if (iter.get_name() == section_name) {
				return remove_section(&iter);
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to remove non-existant section %s in file %s", section_name, path);
		return false;
	}

	bool IniFile::remove_section(IniSection* section) {
		if (section->parent != this) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to remove section %s in file %s, using file %s", section->name, section->parent->get_path(), path);
			return false;
		}

		sections.remove(*section);
		return true;
	}

	string IniFile::get_path() const {
		return path;
	}

	vector<string> IniFile::get_section_names() const {
		vector<string> section_names{};

		for (auto& iter : sections) {
			section_names.push_back(iter.name);
		}

		return section_names;
	}

	string IniFile::get_key_value(string section_name, string key_name) const {
		const IniSection* section = get_section(section_name);

		if (section == nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to get key %s from non-existant section %s in file %s", key_name, section_name, path);
			return "";
		}

		return section->get_key_value(key_name);
	}

	bool IniFile::set_key_value(string section_name, string key_name, string key_value) {
		IniSection* section = get_section(section_name);

		if (section == nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Ini-Error. Trying to set key %s from non-existant section %s in file %s", key_name, section_name, path);
			return false;
		}

		return section->set_key_value(key_name, key_value);
	}
}