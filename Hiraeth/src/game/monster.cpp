#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(const SRL::MonsterData& monster_data, SRL::Summon summon, map::MapLayer* mapLayer)
			: Creature(maths::Rectangle(summon.position, maths::vec2(50, 50)), mapLayer,
				//new MonsterStats(monster_data.StatsStruct), false),
				//&m_MonsterStats, false),
				monster_data.StatsStruct.Speed, monster_data.StatsStruct.Jump,
				&m_MonsterStats, false),
			gen(rd()),
			dis(0, 8),
			m_Hp(maths::vec2(10, 40), 30, 6, 0xff0000ff),
			m_MonsterStats{monster_data.StatsStruct},
			m_XStart(NULL),
			m_XEnd(NULL),
			m_StatsStruct{&m_MonsterStats.m_Stats},
			m_Summon(summon)
		{

			SRL::MonsterTexturesData mtd = SRL::deserial<SRL::MonsterTexturesData>("serialized/monster.data",
				(100 + monster_data.TextureIndex));

			m_HitBox = mtd.creature_sprites.hit_box;
			m_HitSprite = graphics::Sprite{ maths::vec2(0, 0), m_HitBox.x, m_HitBox.y, 0xb066ccff };

			m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.creature_sprites.sprited_data[SRL::MoveState::Stand], 
				graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_stand", mtd.textures_dict[SRL::MoveState::Stand])));
			m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.creature_sprites.sprited_data[SRL::MoveState::Walk],
				graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_walk", mtd.textures_dict[SRL::MoveState::Walk])));
			m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.creature_sprites.sprited_data[SRL::MoveState::Hit], 
				graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_hit", mtd.textures_dict[SRL::MoveState::Hit])));

			m_Org = maths::vec2{ m_HitBox.x / 2, 0 };
			//m_Controls.left = true;
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
				if (m_AiTimer.hasExpired())
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

			m_Animations.update();
			m_Animations.clear_done();
			Creature::update();
		}

		void Monster::draw(graphics::Renderer* renderer) const
		{
			Creature::draw(renderer);
			renderer->push(m_TransformationMatrix);
			m_Animations.draw(renderer);
			renderer->submit(&m_Hp);
			renderer->pop();
		}

		bool Monster::check_collision(const maths::Rectangle& rec) 
		{
			maths::Rectangle hit_box{ m_Bounds.position, m_HitBox };
			//if (m_PrintTimer.isExpired())
			//{
			//	std::cout << "my name is : " + std::to_string(hit_box.x) + ", " + std::to_string(hit_box.y) + ", " + std::to_string(hit_box.width) + ", " + std::to_string(hit_box.height) << "\n";
			//	std::cout << "my car name is : " + std::to_string(rec.x) + ", " + std::to_string(rec.y) + ", " + std::to_string(rec.width) + ", " + std::to_string(rec.height) << "\n";
			//	m_PrintTimer.reSet(1.0f);
			//}
			return ((rec.x < hit_box.x + hit_box.width) && (hit_box.x < rec.x + rec.width)
				&& (rec.y < hit_box.y + hit_box.height) && (hit_box.y < rec.y + rec.height));
		}

		void Monster::getHit(Direction dir, Damage damage, std::unique_ptr<graphics::SpritedRenderable> hit_animation)
		{
			Creature::getHit(dir, damage);
			m_Animations.add(std::move(hit_animation));

		}
		void Monster::getHit(Direction dir, Damage damage)
		{
			Creature::getHit(dir, damage);
		}

		void Monster::cause_damage(Damage damage)
		{
			m_Stats->causeDamage(damage);
			//float hp = m_Stats->getHp();
			//float hp = m_Stats->getMaxHp();
			if (m_StatsStruct->Hp > m_StatsStruct->MaxHp || m_StatsStruct->Hp == 0)
			{
				kill_monster();
				return;
			}
			m_Hp = graphics::Sprite(maths::vec2(10, 40), float(m_StatsStruct->Hp) / m_StatsStruct->MaxHp * 30, 6, 0xff0000ff);
		}
		void Monster::kill_monster()
		{
			died = true;
		}
	}
}