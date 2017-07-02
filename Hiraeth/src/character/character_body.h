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
			CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin)
				: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origin)
			{

			}
			CharacterBody(unsigned int frames_amount, std::vector<float> frames_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin)
				: SpritedRenderable(maths::vec2(0), frames_amount, frames_delay, is_loop, ptex, std::vector<maths::vec2>(frames_amount, origin))
			{

			}
		};
	}
}

