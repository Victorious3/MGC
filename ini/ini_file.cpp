#include "stddef.h"

#include <algorithm>

#include "../filesystem.h"
#include "../ini.h"

using std::ifstream;

namespace ini {
	IniFile::IniFile() {
	}

	IniFile::IniFile(string path) {
		IniParser::fill_ini_file(path, *&*this);
		//if (!load_file(path))
		//	throw runtime_error("Could not open ini file");
	}

	IniFile::~IniFile() {
	}

	/*bool IniFile::load_file(string path) {
		this->path = path;
		ifstream file(path);

		if (!file) {
			SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Could not open ini file %s", path.c_str());
			return false;
		}

		enum ParseState {
			NEW_LINE,
			COMMENT,
			CATEGORY,
			VALUE
		} mode = NEW_LINE;

		IniSection* current_cat = nullptr;
		char c;
		string buffer = "";
		IniKey* current_key = nullptr;
		int line_number = 0;

		auto line_end = [&]() -> bool {
			if (mode == CATEGORY) {
				SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Malformed ini file %s, new line while reading section header name. Line: %u", path, line_number);
				file.close();
				return false;
			}
			else if (mode == VALUE) {
				current_key->set_value(string_tolower(buffer));
				current_key = nullptr;
			}
			return true;
		};

		while (file.get(c)) {
			if (c == '\r' || c == '\n') {
				if (c == '\r' && file.peek() == '\n') {
					file.get(c);
				}
				if (!line_end()) return false;

				buffer = "";
				line_number++;
				mode = NEW_LINE;
				continue;
			}

			if (mode == NEW_LINE) {
				// Reading new line
				switch (c) {
				case ';':
					mode = COMMENT;
					break;
				case ' ':
					buffer += c;
					break;
				case '[':
					buffer = "";
					mode = CATEGORY;
					continue;
					break;
				case '=':
					if (current_cat == nullptr) {
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Malformed ini file %s, key definition without category. Line: %u", path, line_number);
						file.close();
						return false;
					}
					current_key = current_cat->add_key(string_tolower(buffer));
					buffer = "";
					mode = VALUE;
					continue;
					break;
				default:
					buffer += c;
					continue;
				}
			}

			if (mode == COMMENT) {
				// Comment line, ignore all
				continue;
			}
			else if (mode == CATEGORY) {
				// Section heading
				if (c == ']') {
					buffer = string_trim(buffer);
					if (get_section(buffer) != nullptr) {
						SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "Malformed ini file %s, duplicate section header name %s. Line: %u", path, buffer, line_number);
						file.close();
						return false;
					}
					current_cat = add_section(string_tolower(buffer));
					buffer = "";
					mode = COMMENT; // Ignore rest of line
				}
				else {
					buffer += c;
				}
			}
			else if (mode == VALUE) {
				buffer += c;
				//Reading key's value
				continue;
			}
		}
		return line_end();
	}*/

	bool IniFile::save_file() const {
		return IniParser::save_ini_file(*&*this);
	}

	bool IniFile::save_file(string path) const {
		return IniParser::save_ini_file(path, *&*this);
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

	void IniFile::set_path(string path) {
		this->path = path;
	}
}