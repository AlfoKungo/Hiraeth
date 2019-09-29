#include "monster.h"

namespace hiraeth {
	namespace game {

		Monster::Monster(const SRL::MonsterData& monster_data, maths::vec2 position, map::MapLayer* mapLayer, unsigned int mob_id,
			float x_start, float x_end)
			: Creature(maths::Rectangle(position, maths::vec2(50, 50)), mapLayer,
				//new MonsterStats(monster_data.StatsStruct), false),
				//&m_MonsterStats, false),
				monster_data.StatsStruct.Speed / 4, monster_data.StatsStruct.Jump,
				&m_MonsterStats, false),
			m_Hp(maths::vec2(10, 40), 30, 6, 0xff0000ff),
			m_XStart(x_start),
			m_XEnd(x_end),
			m_MonsterStats{ monster_data.StatsStruct },
			m_StatsStruct{ &m_MonsterStats.m_Stats },
			m_Id{ mob_id }
			//m_Summon(summon)
		{
			auto mtd = SRL::deserial<SRL::MonsterTexturesData>("monster",
				(100 + monster_data.TextureIndex));

			m_HitBox = mtd.creature_sprites.hit_box;
			m_HitSprite = graphics::Sprite{ maths::vec2(0, 0), m_HitBox.x, m_HitBox.y, 0xb066ccff };

			m_StatesRenderables[StcStand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
				monster_data.StatsStruct.Name + "_stand", mtd.creature_sprites.sprited_data[SRL::MoveState::Stand]));
			m_StatesRenderables[StcWalk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
				monster_data.StatsStruct.Name + "_walk", mtd.creature_sprites.sprited_data[SRL::MoveState::Walk]));
			m_StatesRenderables[StcJump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
				monster_data.StatsStruct.Name + "_hit", mtd.creature_sprites.sprited_data[SRL::MoveState::Hit]));
			m_StatesRenderables[StcDie].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
				monster_data.StatsStruct.Name + "_die", mtd.creature_sprites.sprited_data[SRL::MoveState::Die], true));

			m_Org = maths::vec2{ m_HitBox.x / 2, 0 };
			//m_Controls.left = true;
			std::srand(std::time(nullptr));
		}

		void Monster::update()
		{
			if (m_Foothold != NO_FOOTHOLD)
			{
				//const physics::FootHold& foothold = m_MapLayer->getFootHolds().at(m_Foothold);
				//physics::FootHold foothold{ m_MapLayer->getFootHolds().at(m_Foothold) };
				//foothold.p1 += 22.5f;
				//foothold.p2 -= 22.5f;
				//if (m_XStart == NULL && m_XEnd == NULL)
				//{
				//	m_XStart = -800;
				//	m_XEnd = 800;
				//	//m_XStart = foothold.getXStart();
				//	//m_XEnd = foothold.getXEnd();
				//}
				//const float&& edge_length = (foothold.p2.x - foothold.p1.x) * 0.2;
				const float edge_length = (m_XEnd - m_XStart) * 0.2f;
				//if (m_Bounds.x < (m_XStart + edge_length))
				if (m_Bounds.x < (m_XStart + edge_length))
				{
					//move({ ((m_XStart + edge_length) - m_Bounds.x) * 2, 0 });
					m_Controls.left = false;
					m_Controls.right = true;
				}
				if (m_Bounds.x > (m_XEnd - edge_length))
				{
					//move({ -(m_Bounds.x - (m_XEnd - edge_length)) * 2 , 0 });
					m_Controls.left = true;
					m_Controls.right = false;
				}
			}

			m_Animations.update();
			m_Animations.clear_done();
			m_ProjectileAnimations.update();
			//for (auto it = m_ProjectileAnimations.m_Renderables.begin(); it != m_ProjectileAnimations.m_Renderables.end() /* not hoisted */; /* no increment */)
		//auto iter = m_ProjectileAnimations.m_Renderables.begin();
		//auto endIter = m_ProjectileAnimations.m_Renderables.end();
		//for (; iter != endIter; )
			for (auto iter = m_ProjectileAnimations.m_Renderables.begin(); iter != m_ProjectileAnimations.m_Renderables.end();)
			{
				if ((*iter)->hasHitClashed())
				{
					m_Animations.add(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0,0 },
						(*iter)->getSkillName() + "_hit", (*iter)->getHitAnimationData(), true));
					//Creature::getHit((*iter)->getDirection(), (*iter)->getDamage());
					(*iter)->m_Attacker->attackMonster(this, (*iter)->getDamage());
					iter = m_ProjectileAnimations.m_Renderables.erase(iter);
				}
				else
					++iter;
			}
			Creature::update();
		}

		void Monster::draw(graphics::Renderer* renderer) const
		{
			Creature::draw(renderer);
			renderer->push(m_TransformationMatrix);
			m_Animations.draw(renderer);
			renderer->submit(&m_Hp);
			renderer->pop();
			m_ProjectileAnimations.draw(renderer);
		}

		bool Monster::check_collision(const maths::Rectangle& rec)
		{
			maths::Rectangle hit_box{ m_Bounds.position, m_HitBox };
			return ((rec.x < hit_box.x + hit_box.width) && (hit_box.x < rec.x + rec.width)
				&& (rec.y < hit_box.y + hit_box.height) && (hit_box.y < rec.y + rec.height));
		}

		void Monster::setHitAnimation(std::unique_ptr<graphics::SpritedRenderable> hit_animation)
		{
			m_Animations.add(std::move(hit_animation));
		}
		void Monster::setStrikeAnimation(std::string skill_name,
			const SRL::FullAnimationData& hit_animation)
		{
			m_Animations.add(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0,0 },
				skill_name + "_hit", hit_animation, true));

		}
		void Monster::setProjectileAnimation(std::unique_ptr<skills::TargetedProjectile> projectile_animation)
		{
			m_ProjectileAnimations.add(std::move(projectile_animation));
		}
		void Monster::getHit(Direction dir, Damage damage)
		{
			Creature::getHit(dir, damage);
		}

		void Monster::cause_damage(Damage damage)
		{
			m_Stats->causeDamage(damage);
			if (m_StatsStruct->Hp > m_StatsStruct->MaxHp || m_StatsStruct->Hp == 0)
			{
				m_StatsStruct->Hp = 0;
				triggerDeathAnimation();
			}
			m_Hp = graphics::Sprite(maths::vec2(10, 40), float(m_StatsStruct->Hp) / m_StatsStruct->MaxHp * 30, 6, 0xff0000ff);
		}
		void Monster::triggerDeathAnimation()
		{
			change_stance(StcDie);
		}
	}
}
