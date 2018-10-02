#pragma once
#include "srl/sprite_data.h"
#include "graphics/renderable.h"
#include "creature.h"

namespace hiraeth {
	namespace skills {
		

		class Projectile : public graphics::SpritedRenderable
		{
			//enum BallState
			//{ inAir, clash} m_BallState;
			game::Creature* m_Enemy;
			maths::vec2 m_OrgPos;
			float m_ProjTime{ 0.75f };
			float m_Speed{ 15.0f };
			ATimer m_Timer;
			std::string m_SkillName;
			SRL::FullAnimationData m_HitAnimationData;
			game::Damage m_Damage;
			game::Direction m_Direction;
			maths::vec2 m_EndPosition;
		public:
			Projectile(maths::vec2 org_pos, game::Direction direction,
				const std::string& skill_name, SRL::FullAnimationData animation_data)
				: SpritedRenderable(org_pos, skill_name + "_projectile", animation_data, false),
			m_EndPosition(org_pos + direction * maths::vec2{500, 0}),
				m_Enemy(nullptr),
				m_OrgPos(org_pos),
				m_Timer(m_ProjTime),
			m_SkillName{skill_name},
			m_Direction{direction}
			{
				
			}

			Projectile(maths::vec2 org_pos, game::Creature* enemy, game::Damage damage, game::Direction direction,
				const std::string& skill_name, SRL::FullAnimationData animation_data, 
				SRL::FullAnimationData hit_data)
				: SpritedRenderable(org_pos, skill_name + "_projectile", animation_data, false),
				m_Enemy(enemy),
				m_OrgPos(org_pos),
				m_Timer(m_ProjTime),
			m_SkillName{skill_name},
			m_HitAnimationData{hit_data},
			m_Damage{damage},
			m_Direction{direction}
			{
			}
			void update() override
			{
				//maths::vec2 Direction = (m_Enemy->getPosition() - m_OrgPos);

				if (m_Enemy != nullptr)
					m_EndPosition = m_Enemy->getBounds().GetMiddle();
				const maths::vec2 destination_vec = m_EndPosition - getPosition();

				const float x_force = m_Speed * copysignf(1.0f, destination_vec.x);
				const float y_force = m_Speed * destination_vec.y / abs(destination_vec.x);

				maths::vec2 new_pos = getPosition() + maths::vec2{ x_force, y_force };
				//maths::vec2 new_pos = { m_Speed * copysignf(1.0f, Direction.x), m_Speed * Direction.y / Direction.y };
				//const auto new_pos = m_OrgPos + (m_Enemy->getPosition() - m_OrgPos) * ((m_ProjTime - m_Timer.timeRemain()) / m_ProjTime);
				setPosition(new_pos);
				SpritedRenderable::update();
			}
			bool hasHitClashed() const
			{
				//return maths::vec2{ m_Enemy->getBounds().GetMiddle() - getPosition() }.MagnitudeU() < 200.0f;
				return maths::vec2{ m_EndPosition - getPosition() }.MagnitudeU() < 200.0f;
			}
			SRL::FullAnimationData getAnimationData()
			{
				return m_HitAnimationData;
			}
			std::string getSkillName() const
			{
				return m_SkillName;
			}

			game::Damage getDamage() const
			{
				return m_Damage;
			}
			game::Direction getDirection() const
			{
				return m_Direction;
			}
			bool hasSpriteFinished() const override
			{
				//return m_Timer.hasExpired();
				return maths::vec2{ m_EndPosition - getPosition() }.MagnitudeU() < 200.0f;
			}
		};
	}
}
