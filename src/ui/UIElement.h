#pragma once

#include <stdafx.h>

namespace UI {
	class UIElement
	{
	public:
		virtual ~UIElement() {}

		virtual void update(Uint64 delta_ms) = 0;
		virtual void render() = 0;
	};
}
