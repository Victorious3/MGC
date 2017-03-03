#include "ini_key.h"

#include "ini_section.h"

string ini::IniKey::get_value() const {
	return value;
}

void ini::IniKey::set_value(string value) {
	this->value = value;
}

bool ini::IniKey::rename(string new_name) {
	return parent->rename_key(this, new_name);
}

string ini::IniKey::get_name() const {
	return name;
}

ini::IniSection* ini::IniKey::get_parent() const {
	return parent;
}

bool ini::operator==(const IniKey& k1, const IniKey& k2) {
	return (k1.get_name() == k2.get_name() &&
		k1.get_value() == k2.get_value() &&
		k1.get_parent() == k2.get_parent());
}