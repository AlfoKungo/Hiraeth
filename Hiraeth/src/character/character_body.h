#pragma once
#include "graphics/sprited_renderable.h"

namespace hiraeth {
	namespace character {
		class CharacterBody : public graphics::SpritedRenderable
		{
			struct BodyConfigs
			{
				maths::vec2 org;
			};
		public:
			CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin);
			CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2* origins_array);

		};
	}
}