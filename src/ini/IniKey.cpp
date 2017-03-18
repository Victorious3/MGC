#include <stdafx.h>

#include "IniKey.h"
#include "IniSection.h"

namespace ini {

	bool IniKey::rename(string new_name) {
		return parent->rename_key(this, new_name);
	}

	bool operator==(const IniKey& k1, const IniKey& k2) {
		return (k1.get_name() == k2.get_name() &&
			k1.value == k2.value &&
			k1.parent == k2.parent);
	}
}