#include <stdafx.h>

#include "../log.h"
#include "../ini/ini.h"
#include "../filesystem.h"

using std::streampos;
using std::ifstream;
using std::ofstream;

namespace ini {

	static ParsedLine parse_line(string line) {
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

	void save_ini_file(const IniFile& ini_file) {
		save_ini_file(ini_file.get_path(), ini_file);
	}

	static inline void write_str(ofstream& stream, string str) {
		stream.write(str.c_str(), str.length());
	}

	void save_ini_file(const string& path, const IniFile& ini_file) {
		ofstream file;

		file.open(path, ofstream::out);
		if (!file.is_open()) {
			throw RUNTIME_ERROR("Could not open file "s + path + " for writing.");
		}

		for (auto& section_name : ini_file.get_section_names()) {
			const IniSection* const section = ini_file.get_section(section_name);

			for (auto& comment : section->get_comments()) {
				write_str(file, "; "s + comment + "\n");
			}
			write_str(file, "["s + section->get_name() + "]\n");

			for (auto& key_name : section->get_key_names()) {
				const IniKey* const key = section->get_key(key_name);

				for (auto& comment : key->comments) {
					write_str(file, "; "s + comment + "\n");
				}

				write_str(file, key->get_name() + "=" + key->value + "\n");
			}
		}

		write_str(file, "\n");
		file.flush();
		file.close();
	}

	void fill_ini_file(string path, IniFile& ini_file) {
		ini_file.path = path;

		ifstream file;
		file.open(path, ifstream::in);

		if (!file.is_open()) {
			throw RUNTIME_ERROR("Could not open ini file: "s + path);
		}

		string line;
		IniSection* section = nullptr;
		vector<string> comments;
		fs::read_line(file, line);
		while (!file.eof()) {
			ParsedLine parsed_line = parse_line(line);

			switch (parsed_line.type) {
			case ParsedLineType::SECTION:
				section = &ini_file.add_section(parsed_line.params[0]);
				section->comments = comments;
				comments.clear();
				break;
			case ParsedLineType::KEY:
				if (section == nullptr) {
					mgc::log::error("ini") << "Skipping key " << parsed_line.params[0] << " = " << parsed_line.params[1] << " while reading file " << path << ", no section set." << endl;
				}
				else {
					IniKey& key = section->add_key(parsed_line.params[0]);
					key.value = parsed_line.params[1];
					key.comments = comments;
					comments.clear();
				}
				break;
			case ParsedLineType::INVALID:
				mgc::log::error("ini") << "Skipping invalid line while reading file " << path << ". Line content: " << line << endl;
				break;
			case ParsedLineType::COMMENT:
				comments.push_back(parsed_line.params[0]);
				break;
			default:
				break;
			}

			fs::read_line(file, line);
		}

		file.close();
	}
}