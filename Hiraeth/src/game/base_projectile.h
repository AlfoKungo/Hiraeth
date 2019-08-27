#pragma once
#include <utility>
#include "srl/sprite_data.h"
#include "graphics/renderable.h"
#include "creature.h"

namespace hiraeth {
	namespace skills {

		//const float m_ProjTime{ 0.75f };
		const float m_Speed{ 15.0f };

		class BaseProjectile : public graphics::SpritedRenderable
		{
			maths::vec2 m_OrgPos;
			std::string m_SkillName;
		protected:
			game::Direction m_Direction;
			maths::vec2 m_EndPosition{};
			ATimer m_Delay;
		public:
			BaseProjectile(maths::vec2 org_pos, game::Direction direction,
				const std::string& skill_name, SRL::FullAnimationData animation_data, float delay)
				: SpritedRenderable(org_pos, skill_name + "_projectile", animation_data, false),
				//m_EndPosition(org_pos + direction * maths::vec2{ 500, 0 }),
				//m_Enemy(nullptr),
				m_OrgPos(org_pos),
				//m_Timer(m_ProjTime),
				m_SkillName{ skill_name },
				m_Direction{ direction },
				m_Delay(delay)
			{
			}

			void update() override
			{
				//maths::vec2 Direction = (m_Enemy->getPosition() - m_OrgPos);
				//maths::vec2 new_pos = { m_Speed * copysignf(1.0f, Direction.x), m_Speed * Direction.y / Direction.y };
				//const auto new_pos = m_OrgPos + (m_Enemy->getPosition() - m_OrgPos) * ((m_ProjTime - m_Timer.timeRemain()) / m_ProjTime);
				if (!m_Delay.hasExpired())
					return;

				const maths::vec2 destination_vec = m_EndPosition - getPosition();

				const float x_force = m_Speed * copysignf(1.0f, destination_vec.x);
				const float y_force = m_Speed * destination_vec.y / abs(destination_vec.x);

				maths::vec2 new_pos = getPosition() + maths::vec2{ x_force, y_force };
				setPosition(new_pos);
				SpritedRenderable::update();
			}
			void draw(graphics::Renderer* renderer) const override
			{
				if (m_Delay.hasExpired())
					SpritedRenderable::draw(renderer);
			}
			bool hasHitClashed() const
			{
				return maths::vec2{ m_EndPosition - getPosition() }.BiggerThan(14);
			}
			//SRL::FullAnimationData getAnimationData()
			//{
			//	return m_HitAnimationData;
			//}
			std::string getSkillName() const
			{
				return m_SkillName;
			}

			//virtual game::Damage getDamage() const = 0;
			//{
			//	return m_Damage;
			//}
			game::Direction getDirection() const
			{
				return m_Direction;
			}
			bool hasSpriteFinished() const override
			{
				return maths::vec2{ m_EndPosition - getPosition() }.BiggerThan(14);
			}
		};
	}
}
