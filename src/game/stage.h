#pragma once

#include <stdafx.h>

#include "Actor.h"
#include "Entity.h"
#include "player.h"
#include "../render/TextureAtlas.h"

namespace mgc {
	class Layer {
		vector<Entity> tiles;
	};

	class Stage {
		sol::table data;
		Layer layers[3];
		render::TextureAtlas cached_sprites;
	
	public:	
		vector<Actor> actors;
		Player player;
	
		Layer& foreground = layers[0];
		Layer& center     = layers[1];
		Layer& background = layers[2];

		void render(Uint64 delta);
		void tick();

		void read(std::istream& in);
		void write(std::ostream& out);
	};
}