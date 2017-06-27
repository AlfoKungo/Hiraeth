#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(MonsterData monster_data, maths::vec2 pos, unsigned int type, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(pos, maths::vec2(50, 50)), mapLayer, MONSTER_SPEED, CHARACTER_JUMP,
				new MonsterStats(monster_data.stats)),
			m_Type(type),
			gen(rd()),
			dis(0, 8),
			m_AiTimer(StaticTimer::timer.elapsed()),
			m_Hp(10, 40, float(m_Stats->Hp) / m_Stats->MaxHp * 30, 6, 0xff0000ff),
			m_StartingPosition(pos)
		{
			srand(time(nullptr));
			m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), monster_data.stand_frames, 0.6f, false, graphics::TextureManager::Load("Assets/monsters/" + monster_data.monster_name + "/stand.png"), 0));
			m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), monster_data.walk_frames, 0.2f, true, graphics::TextureManager::Load("Assets/monsters/" + monster_data.monster_name + "/walk.png"), 0));
			m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), monster_data.hit_frames, 0.2f, true, graphics::TextureManager::Load("Assets/monsters/" + monster_data.monster_name + "/hit.png"), 0));
			m_Controls.left = true;
			std::srand(std::time(nullptr));
		}

		void Monster::update()
		{
			if (m_Foothold != NO_FOOTHOLD)
			{
				const physics::FootHold& foothold = m_MapLayer->m_FootHolds->at(m_Foothold);
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
						if (Stand == m_StanceState)
						{
							if (dis(gen) < 6)
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
			void Monster::draw(graphics::Renderer* renderer) const
		{
			Creature::draw(renderer);
				renderer->push(m_TransformationMatrix);
				renderer->submit(&m_Hp);
				renderer->pop();
		}

		bool Monster::checkCollision(const maths::Rectangle& rec) const
		{

			return ((rec.x < m_Bounds.x + m_Bounds.width) && (m_Bounds.x < rec.x + rec.width)
				&& (rec.y < m_Bounds.y + m_Bounds.height) && (m_Bounds.y < rec.y + rec.height));
		}

		void Monster::causeDamage(Damage damage)
		{
			m_Stats->causeDamage(damage);
			if (m_Stats->Hp > m_Stats->MaxHp || m_Stats->Hp == 0)
			{
				killMonster();
				return;
			}
			m_Hp = graphics::Sprite(10, 40, float(m_Stats->Hp) / m_Stats->MaxHp * 30, 6, 0xff0000ff);
		}
		void Monster::killMonster()
		{
			died = true;
		}
	}
}