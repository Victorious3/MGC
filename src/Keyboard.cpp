#include <stdafx.h>
#include "keyboard.h"

namespace mgc {
	Keyboard::Keyboard() {
	}

	Keyboard::Keyboard(const ini::IniFile& ini) {
		read_config(ini);
	}

	Keyboard::~Keyboard() {
	}

	void Keyboard::read_config(const ini::IniFile& ini) {
		actionmap[ACTIONS::TOGGLE_FULLSCREEN].scancodes.clear();

		actionmap[ACTIONS::TOGGLE_FULLSCREEN].scancodes.push_back((Uint8)stoi(ini.get_key_value("key bindings", "toggle_fullscreen")));
	}

	void Keyboard::process_sdl_event(SDL_Event& event) {
	}

	void Keyboard::update() {
		auto keyboard_state = SDL_GetKeyboardState(nullptr);

		for (auto& iter : actionmap) {
			for (auto& scan_iter : iter.second.scancodes) {
				if (keyboard_state[scan_iter]) {
					iter.second.set(true, !iter.second.down());
					break;
				}
				iter.second.set(false, false);
			}
		}
	}

	const Keyboard::Input_Action& Keyboard::get_action(const ACTIONS& action) const {
		return actionmap.at(action);
	}
}