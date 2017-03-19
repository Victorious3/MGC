#pragma once

namespace util {
	static float linear_interpolatef(float start, float end, float progress) {
		return (end - start) * progress + start;
	}
}