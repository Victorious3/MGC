#pragma once

#include <stdafx.h>

#include <chrono>

namespace timing {
	inline Uint64 get_millis() {
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}
}
