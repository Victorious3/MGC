#pragma once

#include <stdafx.h>

#include "UIElement.h"
#include "../Texture.h"

namespace UI {

	class Image : public UIElement
	{
	public:
		Image(int x, int y, string path, render::TextureManager& manager);
		~Image();

		virtual void update();
		virtual void render();
		int x, y;

	private:
		string path;
		render::Texture texture;
	};
}
