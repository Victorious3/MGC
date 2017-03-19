#pragma once

#include <stdafx.h>

namespace mgc {
	class Task {
	public:

		virtual ~Task() {}

		virtual void update(int time) = 0;
		virtual void finish() = 0;

		bool finished = false;
	};
}