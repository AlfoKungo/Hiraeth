#pragma once
#include "base_projectile.h"

namespace hiraeth {
	namespace skills {

		class TargetedProjectile : public BaseProjectile
		{
		public:
			game::Creature* m_Attacker;
			game::Creature* m_Enemy;
		private:
			SRL::FullAnimationData m_HitAnimationData{};
			game::Damage m_Damage;
		public:
			TargetedProjectile(maths::vec2 org_pos, game::Direction direction,
				const std::string& skill_name, 
				game::Creature* attacker, game::Creature* enemy, 
				game::Damage damage,  SRL::FullAnimationData animation_data, 
				 SRL::FullAnimationData hit_data, float delay)
				: BaseProjectile(org_pos, direction, skill_name, animation_data, delay),
				m_Attacker(attacker),
				m_Enemy(enemy),
				m_HitAnimationData{ std::move(hit_data) },
				m_Damage(damage)
			{
				m_EndPosition = m_Enemy->getBounds().GetMiddle();
			}

			void update() override
			{
				m_EndPosition = m_Enemy->getBounds().GetMiddle();

				BaseProjectile::update();
			}

			SRL::FullAnimationData getHitAnimationData() const
			{
				return m_HitAnimationData;
			}
			game::Damage getDamage() const
			{
				return m_Damage;
			}
		};
	}
}
