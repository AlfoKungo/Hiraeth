#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, int next_map, graphics::Window* wind, graphics::Texture* ptex, Map* map, Timer* time, game::Character* character)
			: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, ptex, time),
			m_Wnd(wind), m_Map(map), m_State(State::Change_Map), m_Char(character)
		{
			m_NextMap = next_map;
		}

		Portal::~Portal()
		{

		}

		void Portal::update()
		{
			SpritedRenderable::update();
 			if (m_State == State::Default && m_Wnd->isKeyPressed(GLFW_KEY_SPACE))
			{
				if (m_Position.distance_to(m_Char->getPosition()) <= 50)
				{
					m_Map->change_map(m_NextMap);
					m_State = State::Change_Map;
				}
			}
			else if (m_State == State::Change_Map && !m_Wnd->isKeyPressed(GLFW_KEY_SPACE))
			{
				m_State = State::Default;
			}
		}
	}
}