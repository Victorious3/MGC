#pragma once

#include <stdafx.h>

namespace mgc {
	namespace UI {
		class UIElement
		{
		public:
			virtual ~UIElement() {}

			virtual void update() = 0;
			virtual void render() = 0;
		};
	}
}