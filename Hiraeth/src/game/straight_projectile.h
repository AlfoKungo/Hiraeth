#pragma once
#include "base_projectile.h"

namespace hiraeth {
	namespace skills {
		
		class StraightProjectile : public BaseProjectile
		{
		private:
			game::Damage m_Damage{};
		public:
			StraightProjectile(maths::vec2 org_pos, game::Direction direction,
				const std::string& skill_name, SRL::FullAnimationData animation_data, 
				float range, float delay)
				: BaseProjectile(org_pos, direction, skill_name, animation_data, delay)
			{
				
				m_EndPosition = org_pos + maths::vec2{ direction * range, 0 };
			}

			game::Damage getDamage() const
			{
				return m_Damage;
			}
		};
	}
}
