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
			CharacterBody(SRL::AnimationData animation_data, graphics::Texture* ptex)
				: SpritedRenderable(maths::vec2(0), animation_data, ptex)
			{

			}
		};
	}
}

