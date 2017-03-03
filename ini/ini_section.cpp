#include "stddef.h"
#include "ini_section.h"

namespace ini {
	bool IniSection::rename(string new_name) {
		return parent->rename_section(name, new_name);
	}

	IniKey* IniSection::add_key(string key_name) {
		if (get_key(key_name) != nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to add duplicate key %s to section %s in file %s", key_name, name, parent->get_path());
			return nullptr;
		}

		keys.push_back(IniKey(this, key_name, ""));
		return &keys.back();
	}

	bool IniSection::remove_key(string key_name) {
		for (auto& iter : keys) {
			if (iter.get_name() == key_name) {
				return remove_key(&iter);
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to remove non-existant key %s in section %s in file %s.", key_name, name, parent->get_path());
		return false;
	}

	bool IniSection::remove_key(IniKey* key) {
		if (key->parent != this) {
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to remove key %s in section %s in file %s, using section %s in file %s", key->get_name(), key->get_parent()->get_name(), key->get_parent()->get_parent()->get_path(), name, parent->get_path());
			return false;
		}

		keys.remove(*key);
		return false;
	}

	bool IniSection::rename_key(IniKey* key, string new_name) {
		if (get_key(new_name) != nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to rename key %s to already existing key %s in section %s in file %s.", key->name, new_name, name, parent->get_path());
			return false;
		}

		key->name = new_name;
		return true;
	}

	bool IniSection::rename_key(string old_name, string new_name) {
		for (auto& iter : keys) {
			if (iter.name == old_name) {
				return rename_key(&iter, new_name);
			}
		}

		SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to rename non-existant key %s to %s in section %s in file %s.", old_name, new_name, name, parent->get_path());
		return false;
	}

	const IniKey* IniSection::get_key(string key_name) const {
		for (auto& iter : keys) {
			if (iter.get_name() == key_name) {
				return &iter;
			}
		}

		return nullptr;
	}

	IniKey* IniSection::get_key(string key_name) {
		return const_cast<IniKey*>(
				static_cast<const IniSection*>(this)->get_key(key_name)
			);
	}

	string IniSection::get_key_value(string key_name) const {	
		const IniKey* const key = get_key(key_name);

		if (key == nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to get value of non-existant key %s in section %s in file %s.", key_name, name, parent->get_path());
			return nullptr;
		}

		return key->get_value();
	}

	bool IniSection::set_key_value(string key_name, string key_value) {
		IniKey* key = get_key(key_name);

		if (key == nullptr) {
			SDL_LogError(SDL_LOG_CATEGORY_SYSTEM, "Ini-Error. Trying to set value of non-existant key %s to %s in section %s in file %s.", key_name, key_value, name, parent->get_path());
			return false;
		}

		key->set_value(key_value);
		return true;
	}

	string IniSection::get_name() const {
		return name;
	}

	IniFile* IniSection::get_parent() const {
		return parent;
	}

	bool operator==(const IniSection& s1, const IniSection& s2) {
		return (s1.get_name() == s2.get_name() && s1.get_parent() == s2.get_parent());
	}
}