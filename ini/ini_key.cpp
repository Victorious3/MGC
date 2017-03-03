#include "ini_key.h"

#include "ini_section.h"

string ini::Ini_Key::get_value() const {
	return value;
}

void ini::Ini_Key::set_value(string value) {
	this->value = value;
}

bool ini::Ini_Key::rename(string new_name) {
	return parent->rename_key(this, new_name);
}

string ini::Ini_Key::get_name() const {
	return name;
}

ini::Ini_Section* ini::Ini_Key::get_parent() const {
	return parent;
}

bool ini::operator==(const Ini_Key& k1, const Ini_Key& k2) {
	return (k1.get_name() == k2.get_name() &&
		k1.get_value() == k2.get_value() &&
		k1.get_parent() == k2.get_parent());
}