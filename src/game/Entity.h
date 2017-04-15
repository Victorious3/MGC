#pragma once
#include <stdafx.h>

#include "../Texture.h"

namespace mgc {
	class Entity {
	public:
		Entity(render::Sprite sprite) : sprite(sprite) {};

		int x, y, id;
		render::Sprite sprite;

		Entity();
		~Entity();
	};

}

