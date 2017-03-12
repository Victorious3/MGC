#pragma once

#include "stddef.h"

namespace mgc {
	class Locale
	{
	public:
		Locale(string locale);
		~Locale();

		void set_locale(string locale = "en-us"s);
		string get_locale() const;

		string get_string(const string& name) const;
		string get_raw_string(const string& name) const;

		string parse_string(const string& input) const;

	private:
		string locale;

		void _load_locale();

		map<string, string> locale_strings {};
	};
}