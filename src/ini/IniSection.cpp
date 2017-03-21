#include <stdafx.h>

#include <sstream>

#include "IniSection.h"
#include "../log.h"

namespace ini {
	thread_local IniSection* _current_section;

	bool IniSection::rename(const string new_name) {
		return parent->rename_section(get_name(), new_name);
	}

	IniKey& IniSection::add_key(const string key_name) {
		IniKey* key = get_key(key_name);
		if (key) return *key;

		keys.push_back(IniKey(this, key_name, ""));
		return keys.back();
	}

	bool IniSection::remove_key(const string& key_name) {
		for (auto& iter : keys) {
			if (iter.get_name() == key_name) {
				return remove_key(&iter);
			}
		}

		mgc::log::warn("ini") << "Trying to remove non-existant key " << key_name << " in section " << get_name() << " in file " << parent->get_path();
		return false;
	}

	bool IniSection::remove_key(IniKey* key) {
		if (key->parent != this) {
			mgc::log::warn("ini") << "Trying to remove key " << key->get_name() << " in section " << key->parent->get_name() << " in file " << key->parent->parent->get_path() << " , using section " << get_name() << " in file " << parent->get_path();
			return false;
		}

		keys.remove(*key);
		return false;
	}

	bool IniSection::rename_key(IniKey* key, const string new_name) {
		if (get_key(new_name)) {
			mgc::log::warn("ini") << "Trying to rename key " << key->get_name() << " to already existing key " << new_name << " in section " << get_name() << " in file " << parent->get_path();
			return false;
		}

		key->name = new_name;
		return true;
	}

	bool IniSection::rename_key(const string& old_name, const string new_name) {
		for (auto& iter : keys) {
			if (iter.get_name() == old_name) {
				return rename_key(&iter, new_name);
			}
		}

		mgc::log::warn("ini") << "Trying to rename non-existant key " << old_name << " to " << new_name << " in section " << get_name() << " in file " << parent->get_path();
		return false;
	}

	const IniKey* IniSection::get_key(const string& key_name) const {
		for (auto& iter : keys) {
			if (iter.get_name() == key_name) {
				return &iter;
			}
		}

		return nullptr;
	}

	IniKey* IniSection::get_key(const string& key_name) {
		return const_cast<IniKey*>(
				static_cast<const IniSection*>(this)->get_key(key_name)
			);
	}

	string& IniSection::operator[](const string& key_name) {
		return add_key(key_name).value;
	}

	template<> vector<string> IniSection::get(const string& key_name, vector<string> def) const {
		if (const IniKey* key = get_key(key_name)) {
			vector<string> v;
			std::istringstream stream(key->value);
			string s;
			while (std::getline(stream, s, ',')) {
				v.push_back(s);
			}
			return v;
		}
		return def;
	}

	// Special case: vector
	template<> void IniSection::set(const string& key_name, vector<string> value) {
		string ret;
		if (!value.empty()) {
			for (auto& v = value.begin(); v != value.end() - 1; v++) {
				ret += *v + ",";
			}
			ret += *value.end();
		}
		set(key_name, ret);
	};

	// Special case: string
	/*template<> void IniSection::set(const string& key_name, string value) {
		add_key(key_name).value = value;
	}*/

	template<typename T> 
	static inline T get_number_signed(const IniSection& section, const string& key_name, T def) {
		if (const IniKey* key = section.get_key(key_name)) {
			try { return static_cast<T>(std::stoll(key->value)); }
			catch (...) {}
		}
		return def;
	}

	template<typename T>
	static inline T get_number_unsigned(const IniSection& section, const string& key_name, T def) {
		if (const IniKey* key = section.get_key(key_name)) {
			try { return static_cast<T>(std::stoull(key->value)); }
			catch (...) {}
		}
		return def;
	}

	template<> Uint64 IniSection::get(const string& key_name, Uint64 def) const { return get_number_unsigned<Uint64>(*this, key_name, def); }
	template<> Uint32 IniSection::get(const string& key_name, Uint32 def) const { return get_number_unsigned<Uint32>(*this, key_name, def); }
	template<> Uint16 IniSection::get(const string& key_name, Uint16 def) const { return get_number_unsigned<Uint16>(*this, key_name, def); }
	template<> Uint8  IniSection::get(const string& key_name, Uint8  def) const { return get_number_unsigned<Uint8 >(*this, key_name, def); }

	template<> int64 IniSection::get(const string& key_name, int64 def) const { return get_number_signed<int64>(*this, key_name, def); }
	template<> int32 IniSection::get(const string& key_name, int32 def) const { return get_number_signed<int32>(*this, key_name, def); }
	template<> int16 IniSection::get(const string& key_name, int16 def) const { return get_number_signed<int16>(*this, key_name, def); }
	template<> int8  IniSection::get(const string& key_name, int8  def) const { return get_number_signed<int8 >(*this, key_name, def); }

	template<> float IniSection::get(const string& key_name, float def) const {
		if (const IniKey* key = get_key(key_name)) {
			try { return std::stof(key->value); }
			catch (...) {}
		}
		return def;
	}

	template<> double IniSection::get(const string& key_name, double def) const {
		if (const IniKey* key = get_key(key_name)) {
			try { return std::stod(key->value); }
			catch (...) {}
		}
		return def;
	}

	vector<string> IniSection::get_key_names() const {
		vector<string> names;

		for (auto& key : keys) {
			names.push_back(key.get_name());
		}

		return names;
	}

	bool operator==(const IniSection& s1, const IniSection& s2) {
		return (s1.get_name() == s2.get_name() && s1.parent == s2.parent);
	}
}