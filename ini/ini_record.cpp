#include "stddef.h"
#include "ini_record.h"

namespace ini {
	thread_local IniSection* IniRecord::section;

	IniRecord::IniRecord() {
		if (!section)
			throw runtime_error("No section provided, perhaps the ini file didn't load properly?");
	}
}

