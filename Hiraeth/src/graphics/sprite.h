#pragma once

#include "renderable.h"

namespace hiraeth {
	namespace graphics {

		class Sprite : public Renderable 
		{

		private:
		public:
			Sprite(maths::vec2 pos, float width, float height, const unsigned int color);
			Sprite(maths::vec2 pos, Texture* texture);
			Sprite(maths::vec2 pos, float width, float height, Texture* texture);
			Sprite(maths::vec2 pos, float width, float height, Texture* texture, std::vector<maths::vec2> uv);
		};
	}
}