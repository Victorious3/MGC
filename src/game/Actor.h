#pragma once
#include <stdafx.h>

#include "Entity.h"

namespace mgc {
	class Actor : public Entity {
	public:
		sol::table data;

		void render(Uint64 delta);
		void tick();

		void read(std::istream& in);
		void write(std::ostream& out);
	};
}