#include <stdafx.h>

#include "IniKey.h"
#include "IniSection.h"

namespace ini {

	bool operator==(const IniKey& k1, const IniKey& k2) {
		return (k1.name == k2.name &&
			k1.value == k2.value &&
			k1.parent == k2.parent);
	}
}