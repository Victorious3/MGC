#include <stdafx.h>
#include "keyboard.h"

namespace mgc {
	Keyboard::Keyboard() {
	}

	Keyboard::Keyboard(const ini::IniFile& ini, const string& section_name) {
		read_config(ini, section_name);
	}

	void Keyboard::read_config(const ini::IniFile& ini, const string& section_name) {
		auto section = ini.get_section(section_name);
		if (section) {
			for (auto& kv : actionmap) {
				kv.second.scancode = static_cast<SDL_Scancode>(section->get<Uint>(kv.first, kv.second.scancode));
			}
		}
	}

	void Keyboard::process_sdl_event(SDL_Event& event) {
	}

	void Keyboard::update(Uint64 delta_ms) {
		const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);
		
		for (auto& iter : actionmap) {
			bool last = iter.second.active;
			iter.second.active = keyboard_state[iter.second.scancode];
			iter.second.fired = !last && iter.second.active;
		}
	}

	const Keyboard::InputAction& Keyboard::get_action(const string& name, SDL_Scancode default) {
		return actionmap.emplace(name, InputAction(default)).first->second;
	}
}