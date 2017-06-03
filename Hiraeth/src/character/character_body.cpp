//#include "character_body.h"
//
//namespace hiraeth {
//	namespace character {
//		template <unsigned int N >
//		CharacterBody<N>::CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2 origin)
//			: SpritedRenderable<N>(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origin)
//		{
//
//		}
//		template <unsigned int N >
//		CharacterBody<N>::CharacterBody(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2* origins_array)
//			: SpritedRenderable<N>(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origins_array)
//		{
//
//		}
//		template void CharacterBody<int>();
//	}
//}