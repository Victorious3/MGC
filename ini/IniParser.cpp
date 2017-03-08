#include "stddef.h"

#include "../ini.h"

#include <fstream>
using std::streampos;
using std::ifstream;
using std::ofstream;

namespace ini {
	std::istream& safe_get_line(std::istream& is, std::string& t) {
		t.clear();

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;) {
			int c = sb->sbumpc();
			switch (c) {
			case '\n':
				return is;
				break;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
				break;
			case EOF:
				if (t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += (char)c;
			}
		}
	}

	IniParser::ParsedLine IniParser::_parse_line(string line) {
		string trimmed_line = string_trim(line);

		if (trimmed_line.length() == 0) {
			return ParsedLine{
				ParsedLineType::EMPTY,
				"",
				""
			};
		}

		if (trimmed_line[0] == ';') {
			return ParsedLine{
				ParsedLineType::COMMENT,
				string_trim(trimmed_line.substr(1)),
				""
			};
		}
		else if (trimmed_line[0] == '[') {
			if (trimmed_line[trimmed_line.length() - 1] != ']') {
				// ERROR
				return ParsedLine{
					ParsedLineType::INVALID,
					"",
					""
				};
			}
			return ParsedLine{
				ParsedLineType::SECTION,
				string_trim(trimmed_line.substr(1, trimmed_line.length() - 2)),
				""
			};
		}
		else {
			size_t pos = line.find_first_of('=');

			if (pos != -1) {
				return ParsedLine{
					ParsedLineType::KEY,
					string_trim(line.substr(0, pos)),
					line.substr(pos + 1)
				};
			}
			else {
				return ParsedLine{
					ParsedLineType::INVALID,
					"",
					""
				};
			}
		}
	}

	bool IniParser::save_ini_file(const IniFile& ini_file) {
		return save_ini_file(ini_file.get_path(), ini_file);
	}

	void inline write_str(ofstream& stream, string str) {
		stream.write(str.c_str(), str.length());
	}

	bool IniParser::save_ini_file(const string& path, const IniFile& ini_file) {
		ofstream file{};

		file.open(path, ofstream::out);
		if (!file.is_open()) {
			throw runtime_error("Could not open file "s + path + " for writing.");
		}

		for (auto& section_name : ini_file.get_section_names()) {
			const IniSection* const section = ini_file.get_section(section_name);

			for (auto& comment : section->get_comments()) {
				write_str(file, "; "s + comment + "\n");
			}
			write_str(file, "["s + section->get_name() + "]\n");

			for (auto& key_name : section->get_key_names()) {
				const IniKey* const key = section->get_key(key_name);

				for (auto& comment : key->get_comments()) {
					write_str(file, "; "s + comment + "\n");
				}

				write_str(file, key->get_name() + "=" + key->get_value() + "\n");
			}
		}

		write_str(file, "\n");
		file.flush();
		file.close();
	}

	void IniParser::fill_ini_file(string path, IniFile& ini_file) {
		ini_file.set_path(path);

		ifstream file{};
		file.open(path, ifstream::in);

		if (!file.is_open()) {
			throw runtime_error("Could not open ini file: "s + path);
		}

		string line;
		IniSection* section = nullptr;
		vector<string> comments{};
		safe_get_line(file, line);
		while (!file.eof()) {
			ParsedLine parsed_line = _parse_line(line);

			switch (parsed_line.type) {
			case ParsedLineType::SECTION:
				section = ini_file.add_section(parsed_line.params[0]);
				section->set_comments(vector<string>{comments});
				comments.clear();
				break;
			case ParsedLineType::KEY:
				if (section == nullptr) {
					SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Skipping key %s = %s while reading file %s, no section set.", parsed_line.params[0], parsed_line.params[1], path);
				}
				else {
					IniKey* key = section->add_key(parsed_line.params[0]);
					key->set_value(parsed_line.params[1]);
					key->set_comments(vector<string>{comments});
					comments.clear();
				}
				break;
			case ParsedLineType::INVALID:
				SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Skipping invalid line while reading file %s. Line content: %s", path, line);
				break;
			case ParsedLineType::COMMENT:
				comments.push_back(parsed_line.params[0]);
				break;
			default:
				break;
			}

			safe_get_line(file, line);
		}

		file.close();
	}

	IniFile IniParser::load_ini_file(string path) {
		IniFile ini_file{};

		fill_ini_file(path, ini_file);

		return ini_file;
	}
}