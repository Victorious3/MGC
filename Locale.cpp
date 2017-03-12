#include "stddef.h"

#include "Locale.h"
#include "filesystem.h"

namespace mgc {
	Locale::Locale(string locale) {
		this->locale = locale;

		_load_locale();
	}

	Locale::~Locale()
	{
	}

	void Locale::_load_locale() {
		locale_strings.clear();
		ifstream file("Resources/lang/" + locale + ".cfg");

		if (!file.is_open()) {
			throw runtime_error("Could not open locale: " + locale);
		}

		string line = "";
		fs::read_line(file, line);
		
		while (!file.eof()) {
			size_t pos = line.find_first_of('=', 0);
			
			if (pos != -1) {
				string key = line.substr(0, pos);
				string value = line.substr(pos + 1);

				locale_strings[key] = value;
			}

			fs::read_line(file, line);
		}

		file.close();
	}

	void Locale::set_locale(string locale) {
		this->locale = locale;

		_load_locale();
	}

	string Locale::get_locale() const {
		return locale;
	}

	string Locale::get_raw_string(const string& name) const {
		return locale_strings.at(name);
	}

	string Locale::get_string(const string& name) const {
		return parse_string(locale_strings.at(name));
	}

	string Locale::parse_string(const string& input) const {
		string output = "";

		bool escaped = false;
		bool parsing = false;
		string buffer = "";
		size_t p = 0;
		while (p < input.size()) {
			char c = input[p];

			if (escaped) {
				if (!parsing) {
					output += c;
				}
				else {
					buffer += c;
				}
				escaped = false;
			}
			else {
				switch (c) {
				case '\\': {
					escaped = true;
				} break;
				case '{': {
					parsing = true;
				} break;
				case '}': {
					if (parsing) {
						parsing = false;
						output += get_string(buffer);
						buffer = "";
					}
					else {
						buffer += c;
					}
				} break;
				default: {
					if (!parsing) {
						output += c;
					}
					else {
						buffer += c;
					}
				} break;
				}
			}

			p++;
		}

		return output;
	}
}