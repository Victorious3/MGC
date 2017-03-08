#pragma once

#include "../stddef.h"

namespace ini {
	class IniFile;

	class IniParser {
	public:
		enum class ParsedLineType {
			EMPTY,
			SECTION,
			COMMENT,
			KEY,
			INVALID
		};

		struct ParsedLine {
			ParsedLineType type = ParsedLineType::EMPTY;
			string params[2] = { "", "" };

			ParsedLine(ParsedLineType type, string param1, string param2) {
				this->type = type;
				this->params[0] = param1;
				this->params[1] = param2;
			}
		};

	private:
		static ParsedLine _parse_line(string line);

	public:

		IniParser() = delete;
		~IniParser() = delete;

		static bool save_ini_file(const IniFile& ini_file);
		static bool save_ini_file(const string& path, const IniFile& ini_file);
		static IniFile load_ini_file(string path);
		static void fill_ini_file(string path, IniFile& ini_file);
	};
}
