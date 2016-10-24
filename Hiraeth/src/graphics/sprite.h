#pragma once

#include "renderable2d.h"

namespace hiraeth {
	namespace graphics {

		class Sprite : public Renderable2D 
		{

		private:
		public:
			Sprite(float x, float y, float width, float height,const unsigned int color);
			Sprite(float x, float y, float width, float height, Texture* texture);
			Sprite(float x, float y, float width, float height, Texture* texture, std::vector<maths::vec2> uv);
		};
	}
}