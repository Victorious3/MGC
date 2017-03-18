#include <stdafx.h>

#include "UI.h"
#include "UIElement.h"

namespace mgc {
	namespace UI {
		void update() {
			for (auto& element : elements) {
				element->update();
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
}