#pragma once
#include "graphics/sprited_renderable.h"

namespace hiraeth {
	namespace character {
		template <unsigned int N >
		class CharacterBody : public graphics::SpritedRenderable<N>
		{
			struct BodyConfigs
			{
				maths::vec2 org;
			};
		public:
			CharacterBody(float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin)
				: SpritedRenderable<N>(maths::vec2(0), frame_delay, is_loop, ptex, origin)
			{

			}
			CharacterBody(float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2* origins_array)
				: SpritedRenderable<N>(maths::vec2(0), frame_delay, is_loop, ptex, origins_array)
			{

			}
		};
	}
}

