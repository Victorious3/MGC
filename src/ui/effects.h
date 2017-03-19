#pragma once

#include <stdafx.h>

namespace UI {
	namespace effects {
		enum class Effects {
			SCREEN_FADE_BLACK,
			NUM_ITEMS
		};

		struct Effect {
			int start_time;
			int time;
			float start_val;
			float target_val;
			Effects effect;
		};

		static float interpolatef(const Effect& effect, const int& current_time) {
			if (current_time > effect.start_time + effect.time)
				return effect.target_val;

			return ((current_time - effect.start_time) / effect.time) * (effect.target_val - effect.start_val) + effect.start_val;
		}

		vector<Effect> effects;

		static void init() {
			for (size_t i = 0; i < static_cast<size_t>(Effects::NUM_ITEMS); ++i) {
				effects.push_back(Effect{ 0, 0, 0, 0, static_cast<Effects>(i) });
			}
		}

		static void update(int time) {
			for (auto& effect : effects) {
				if (time < effect.start_time + effect.time) {
					switch (effect.effect) {
					case Effects::SCREEN_FADE_BLACK:
					{
						// something something
					} break;
					}
				}
			}
		}
	}
}