#include "character_body.h"

namespace hiraeth {
	namespace character {
		CharacterBody::CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin)
			: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origin)
		{

		}
		CharacterBody::CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2* origins_array)
			: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origins_array)
		{

		}
	}
}