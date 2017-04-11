#pragma once

#include <stdafx.h>

namespace UI {
	namespace Layout {
		enum class Anchor {
			TopLeft,
			Top,
			TopRight,
			Left,
			Center,
			Right,
			BottomLeft,
			Bottom,
			BottomRight
		};
	}

	class UIElement;

	static vector<UIElement*> elements;

	void update(const Uint64& delta_ms);
	void render();

	void push_element(UIElement* element);
}
