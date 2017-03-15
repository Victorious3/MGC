#include <stdafx.h>

#include "IniKey.h"
#include "IniSection.h"

namespace ini {
	string IniKey::get_value() const {
		return value;
	}

	void IniKey::set_value(string value) {
		this->value = value;
	}

	bool IniKey::rename(string new_name) {
		return parent->rename_key(this, new_name);
	}

	string IniKey::get_name() const {
		return name;
	}

	IniSection* IniKey::get_parent() const {
		return parent;
	}

	void IniKey::set_comments(vector<string> comments) {
		this->comments = comments;
	}

	vector<string> IniKey::get_comments() const {
		return vector<string>{comments};
	}

	bool operator==(const IniKey& k1, const IniKey& k2) {
		return (k1.get_name() == k2.get_name() &&
			k1.get_value() == k2.get_value() &&
			k1.get_parent() == k2.get_parent());
	}
}