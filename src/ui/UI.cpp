#include <stdafx.h>

#include "UI.h"
#include "UIElement.h"

namespace UI {
	void update(const Uint64& delta_ms) {
		for (auto& element : elements) {
			element->update(delta_ms);
		}
	}

	void render() {
		for (auto& element : elements) {
			element->render();
		}
	}

	void push_element(UIElement* element) {
		elements.push_back(element);
	}
}