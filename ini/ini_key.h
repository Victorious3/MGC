#pragma once

#include "../stddef.h"

namespace ini {
	class Ini_Section;

	class Ini_Key {
		friend Ini_Section;

	private:
		Ini_Key(Ini_Section* parent, string key_name) : parent(parent), name(key_name) {}
		Ini_Key(Ini_Section* parent, string key_name, string key_value) : parent(parent), name(key_name), value(key_value) {}

		Ini_Section* parent;

		string value;
		string name;

	public:

		string get_value() const;
		void set_value(string value);

		bool rename(string new_name);

		string get_name() const;

		Ini_Section* get_parent() const;
	};

	bool operator==(const Ini_Key& k1, const Ini_Key& k2);
}