#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(maths::vec2 pos, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(pos, maths::vec2(45,31)), mapLayer) 
		{
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.6f, true, graphics::TextureManager::Load("slime_stand.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 7, 0.2f, true, graphics::TextureManager::Load("slime_walk.png")));
			m_Controls.left = true;
		}

		void Monster::update()
		{
			if (m_Foothold >= 0)
			{
				if (m_Bounds.x < (m_MapLayer->m_FootHolds.at(m_Foothold).p1.x + 200))
				{
					m_Controls.left = false;
					m_Controls.right = true;
				}
				if (m_Bounds.x > (m_MapLayer->m_FootHolds.at(m_Foothold).p2.x - 200))
				{
					m_Controls.left = true;
					m_Controls.right = false;

				}
			}
			Creature::update();
		}
	}

}