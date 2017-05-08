#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(maths::vec2 pos, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(pos, maths::vec2(45, 31)), mapLayer), timer(0)
		{
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.6f, true, graphics::TextureManager::Load("slime_stand.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 7, 0.2f, true, graphics::TextureManager::Load("slime_walk.png")));
			m_Controls.left = true;
			std::srand(std::time(0));
		}

		void Monster::update()
		{
			if (m_Foothold >= 0)
			{
				const physics::FootHold& foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
				const float&& length = foothold.p2.x - foothold.p1.x;
				if (m_Bounds.x < (foothold.p1.x + length * 0.2))
				{
					m_Controls.left = false;
					m_Controls.right = true;
				}
				if (m_Bounds.x > (foothold.p2.x - length * 0.2))
				{
					m_Controls.left = true;
					m_Controls.right = false;

				}
				if (StaticTimer::timer.elapsed() - timer > 2.0f)
				{
					std::random_device rd;
					std::mt19937 gen(rd());
					std::uniform_int_distribution<> dis(0, 6);
					if (dis(gen) < 1)
					{
						m_Controls.left = false;
						m_Controls.right = false;
					}
					else
					{
						if (StanceState::Stand == m_StanceState)
						{
							if (dis(gen) < 4)
							{
								m_Controls.left = true;
								m_Controls.right = false;
							}
							else
							{
								m_Controls.left = false;
								m_Controls.right = true;
							}
						}
					}
					timer += 2.0f;
				}
			}
			Creature::update();
		}
	}
}