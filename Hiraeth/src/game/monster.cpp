#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(maths::vec2 pos, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(pos, maths::vec2(45, 31)), mapLayer, MONSTER_SPEED, CHARACTER_JUMP,
				new MonsterStats()),
			gen(rd()),
			dis(0, 8),
			m_AiTimer(StaticTimer::timer.elapsed())
		{
			srand(time(NULL));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.6f, true, graphics::TextureManager::Load("slime_stand.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 7, 0.2f, true, graphics::TextureManager::Load("slime_walk.png")));
			m_Controls.left = true;
			std::srand(std::time(0));
		}

		void Monster::update()
		{
			if (m_Foothold != NO_FOOTHOLD)
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
				if (StaticTimer::timer.elapsed() - m_AiTimer > 0.0f)
				{
					if (dis(gen) < 4)
					{
						m_Controls.left = false;
						m_Controls.right = false;
					}
					else
					{
						if (StanceState::Stand == m_StanceState)
						{
							if (dis(gen) < 7)
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
					m_AiTimer += (float)(rand() % 30) / 10 + 1;
					//m_AiTimer += 2.0f;
				}
			}
			Creature::update();
		}

		bool Monster::checkCollision(const maths::Rectangle& rec) const
		{

			return ((rec.x < m_Bounds.x + m_Bounds.width) && (m_Bounds.x < rec.x + rec.width)
				&& (rec.y < m_Bounds.y + m_Bounds.height) && (m_Bounds.y < rec.y + rec.height));
		}
	}
}