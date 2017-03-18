#pragma once

#include <stdafx.h>

#include "UIElement.h"
#include "../Texture.h"

namespace mgc {
	namespace UI {
		class Image : public UIElement
		{
		public:
			Image(int x, int y, string path, TextureManager& manager, Canvas& canvas);
			~Image();

			virtual void update();
			virtual void render();

		private:

			string path;
			Texture texture;
			int x, y;
			Canvas& canvas;
		};
	}
}