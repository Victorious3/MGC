#pragma once
#include <stdafx.h>

namespace ini {
	class IniFile;

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

		ParsedLine(ParsedLineType type, string param1, string param2) 
			: type(type)
			, params { param1, param2 } {}
	};

	void save_ini_file(const IniFile& ini_file);
	void save_ini_file(const string& path, const IniFile& ini_file);
	IniFile load_ini_file(string path);
	void fill_ini_file(string path, IniFile& ini_file);
}
