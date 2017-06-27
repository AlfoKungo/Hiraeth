#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, int next_map)
			: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, false, graphics::TextureManager::Load("portal_adv.png"), maths::vec2(0))
		{
			m_NextMap = next_map;
		}

		Portal::~Portal()
		{

		}
	}
}