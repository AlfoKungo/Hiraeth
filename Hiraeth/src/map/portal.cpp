#include "portal.h"
#include "map.h"

namespace hiraeth {
	namespace map {

		Portal::Portal(maths::vec3 position, int next_map, input::Keyboard* kb, graphics::Texture* ptex, Map* map, game::Character* character)
			: SpritedRenderable(position, 8, PORTAL_ANIMATION_DELAY, ptex),
			m_Kb(kb), m_Map(map), m_State(State::Change_Map), m_Char(character)
		{
			m_NextMap = next_map;
			m_Kb->registerToKey(input::Controls::up, this);
		}

		Portal::~Portal()
		{

		}
		
		void Portal::ButtonClicked(input::Controls control)
		{
			//if (input::Controls::up == control)
			//	if (m_Bounds.GetMinimumBound().Distance(m_Char->getPosition()) <= 100)
			//	{
			//		m_Map->change_map(m_NextMap);
			//	}
		}

		void Portal::update()
		{
			SpritedRenderable::update();
			if (m_State == State::Default && m_Kb->isKeyPressed(GLFW_KEY_UP))
			{
				if (m_Bounds.GetMinimumBound().Distance(m_Char->getPosition()) <= 80)
				{
					m_Map->change_map(m_NextMap);
					m_State = State::Change_Map;
				}
			}
			else if (m_State == State::Change_Map && !m_Kb->isKeyPressed(GLFW_KEY_UP))
			{
				m_State = State::Default;
			}
		}
	}
}