#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, int next_map, input::Keyboard* kb, Map* map, game::Character* character)
			: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, graphics::TextureManager::Load("portal_adv.png"))
		{
			m_NextMap = next_map;
		}

		Portal::~Portal()
		{

		}
	}
}