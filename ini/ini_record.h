#pragma once
#include "../stddef.h"

#define ini_key($x, $def) $x = section->get<decltype($x)>(#$x, ($def))

namespace ini {
	class IniFile;
	class IniSection;

	class IniRecord {
		friend IniSection;
	protected:
		IniRecord();
		thread_local static IniSection* section;
	};
}
