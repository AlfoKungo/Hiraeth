#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(const SRL::MonsterData& monster_data, SRL::Summon summon, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(summon.position, maths::vec2(50, 50)), mapLayer,
				new MonsterStats(monster_data.StatsStruct), false),
			gen(rd()),
			dis(0, 8),
			m_Hp(maths::vec2(10, 40), float(m_Stats->Hp) / m_Stats->MaxHp * 30, 6, 0xff0000ff),
			m_Summon(summon),
			m_XStart(NULL),
			m_XEnd(NULL)
		{
			
			SRL::MonsterTexturesData mtd = SRL::deserial<SRL::MonsterTexturesData>("serialized/monster.data", 
				(100 + monster_data.TextureIndex));

			srand(time(nullptr));
			m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.frames_amount.stand_frames, 0.6f, false, graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_stand", mtd.stand_texture), 0));
			m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.frames_amount.walk_frames, 0.2f, true, graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_walk", mtd.walk_texture), 0));
			m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.frames_amount.hit_frames, 0.2f, true, graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_hit", mtd.hit_texture), 0));
			m_Controls.left = true;
			std::srand(std::time(nullptr));
		}

		void Monster::update()
		{
			if (m_Foothold != NO_FOOTHOLD)
			{
				const physics::FootHold& foothold = m_MapLayer->getFootHolds().at(m_Foothold);
				if (m_XStart == NULL && m_XEnd == NULL)
				{
					m_XStart = foothold.getXStart();
					m_XEnd = foothold.getXEnd();
				}
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
				if (m_AiTimer.isExpired())
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
					m_AiTimer.reSet((float)(rand() % 30) / 10 + 1);
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

		void Monster::getHit(Direction dir, Damage damage)
		{
			Creature::getHit(dir, damage);
		}

		void Monster::causeDamage(Damage damage)
		{
			m_Stats->causeDamage(damage);
			if (m_Stats->Hp > m_Stats->MaxHp || m_Stats->Hp == 0)
			{
				killMonster();
				return;
			}
			m_Hp = graphics::Sprite(maths::vec2(10, 40), float(m_Stats->Hp) / m_Stats->MaxHp * 30, 6, 0xff0000ff);
		}
		void Monster::killMonster()
		{
			died = true;
		}
	}
}