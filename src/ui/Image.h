#pragma once

#include <stdafx.h>

#include "../render.h"
#include "UIElement.h"


namespace UI {

	class Image : public UIElement
	{
	public:
		Image(int x, int y, string path, render::TextureManager& manager = render::texture_manager);

		virtual void update();
		virtual void render();

	private:

		string path;
		render::Texture texture;
		int x, y;
	};
}
