#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, int next_map)
			//: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, false, graphics::TextureManager::Load("portal_adv.png"), maths::vec2(0))
			: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, false, graphics::TextureManager::Load("portal_adv.png")),
			m_NextMap(next_map)

		{
			//SRL::AnimationData ani_data{ {{{0,0,110,150}, {55, 0}, 150}, {{110,0,110,150}, {55, 0}, 150}, 
			//	{{220,0,110,150}, {55, 0}, 150}, {{330,0,110,150}, {55, 0}, 150}, 
			//	{{440,0,110,150}, {55, 0}, 150}, {{550,0,110,150}, {55, 0}, 150}, 
			//	{{660,0,110,150}, {55, 0}, 150}, {{770,0,110,150}, {55, 0}, 150}}, true };
		//m_NextMap = next_map;
		}

	}
}