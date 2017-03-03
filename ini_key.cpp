#include "ini_key.h"

#include "ini_section.h"

string mgc::Ini_Key::get_value() const {
	return value;
}

void mgc::Ini_Key::set_value(string value) {
	this->value = value;
}

bool mgc::Ini_Key::rename(string new_name) {
	return parent->rename_key(this, new_name);
}

string mgc::Ini_Key::get_name() const {
	return name;
}

mgc::Ini_Section* mgc::Ini_Key::get_parent() const {
	return parent;
}

bool mgc::operator==(const Ini_Key& k1, const Ini_Key& k2) {
	return (k1.get_name() == k2.get_name() &&
		k1.get_value() == k2.get_value() &&
		k1.get_parent() == k2.get_parent());
}