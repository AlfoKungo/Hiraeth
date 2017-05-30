#pragma once
#include "graphics/renderer.h"

namespace hiraeth {

		class Drawable
		{
		public:
			bool is_to_draw;
			Drawable() : is_to_draw(true) {}
			virtual void draw(graphics::Renderer* renderer) const = 0;
			virtual ~Drawable() {}
		};
}
