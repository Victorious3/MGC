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

		actionmap[ACTIONS::TOGGLE_FULLSCREEN].scancodes.push_back(ini.get_section("key bindings")->get<Uint>("toggle_fullscreen", SDLK_F11));
	}

	void Keyboard::process_sdl_event(SDL_Event& event) {
	}

	void Keyboard::update() {
		const Uint8* keyboard_state = SDL_GetKeyboardState(nullptr);

		for (auto& iter : actionmap) {
			for (auto& scan_iter : iter.second.scancodes) {
				if (keyboard_state[scan_iter]) {
					iter.second.down = true;
					iter.second.pressed = !iter.second.down;
					break;
				}
				iter.second.down = false;
				iter.second.pressed = false;
			}
		}
	}

	const Keyboard::InputAction& Keyboard::get_action(const ACTIONS& action) const {
		return actionmap.at(action);
	}
}