#pragma once
#include "graphics/renderer2d.h"

namespace hiraeth {

		class Drawable
		{
		public:
			bool is_to_draw = true;
			virtual void draw(graphics::Renderer2D* renderer) const = 0;
			virtual ~Drawable() {}
		};
}
