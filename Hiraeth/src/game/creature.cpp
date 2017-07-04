#include "creature.h"

namespace hiraeth {
	namespace game {

		Creature::Creature(maths::Rectangle bounds, map::MapLayer* mapLayer,
			Stats* stats, bool is_immune_after_hit)
			: Collisionable(m_Bounds, mapLayer), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			m_Stats(stats),
			m_Speed(stats->Speed), m_Jump(stats->Jump),
			m_TransformationMatrix(maths::mat4::Translate(bounds.position)),
			m_IsImmuneAfterHit(is_immune_after_hit)
		{
			m_StatesRenderables[Stand] = std::vector<std::unique_ptr<graphics::Renderable>>();
			m_StatesRenderables[Walk] = std::vector<std::unique_ptr<graphics::Renderable>>();
			m_StatesRenderables[Attack] = std::vector<std::unique_ptr<graphics::Renderable>>();
			m_StatesRenderables[Jump] = std::vector<std::unique_ptr<graphics::Renderable>>();
			m_Bounds = bounds;
		}

		Creature::~Creature()
		{
			//for (auto RenderablesVector : m_StatesRenderables)
			//{
			//	for (auto Renderable : (*RenderablesVector.second))
			//		delete Renderable;
			//	delete (RenderablesVector.second);
			//}
		}

		void Creature::update()
		{

			for (auto & renderable : m_StatesRenderables.at(m_StanceState))
				renderable->update();

			if (is_hit && m_IsImmuneAfterHit)
			{
				if (m_HitTimer.isExpired())
				{
					is_hit = false;
					m_Color = 0xffffffff;
				}
				else
					m_Color = 0x00ffffff | ((unsigned char((m_HitTimer.timeRemain()) / 1.5f * 128 * 2) % 128 + 86) << 24);
			}

			//if (m_Time->elapsed() - m_MovementTimer > CHARACTER_TIME_BETWEEN_ADDS)
			//{
			analyze_controls();
			//m_Force *= (m_Time->elapsed() - m_MovementTimer)*60;
			addGravity(m_Force);
			multiplyByFriction(m_Force);
			analyzeCollision();
			if (Attack != m_StanceState)
			{
				analyze_stance();
			}
			else
				switch (m_AttackState) {
				case PreHit:
					m_Force.x = 0;
					if (m_AttackTimer.isExpired())
					{
						attack();
						m_AttackState = PostHit;
						m_AttackTimer.reSet(0.25f);
					}
					break;
				case PostHit:
					m_Force.x = 0;
					if (m_AttackTimer.isExpired())
					{
						m_AttackState = PostHitMovable;
						m_AttackTimer.reSet(0.1f);
					}
					break;
				case PostHitMovable:
					if (m_AttackTimer.isExpired() || (m_Force.x < -0.1f || m_Force.x > 0.1f))
					{
						m_StanceState = NoStance;
						analyze_controls();
						analyze_stance();
					}
					break;

				}
			//change_stance(Jump);

		//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);

		//m_MovementTimer = StaticTimer::timer.elapsed();
		//}
		}

		void Creature::analyze_controls()
		{
			if (m_Controls.attack && Attack != m_StanceState)
			{
				change_stance(Attack);
				m_AttackState = PreHit;
				m_AttackTimer.reSet(0.05f);
				return;
			}
			if (Attack == m_StanceState && (PreHit == m_AttackState || PostHit == m_AttackState))
				return;
			if (m_Controls.right)
			{
				m_Direction = Right;
				m_Force.x += calculateForce(m_Speed);
			}
			else if (m_Controls.left)
			{
				m_Direction = Left;
				m_Force.x -= calculateForce(m_Speed);
			}
			if (m_Controls.jump && m_Foothold != NO_FOOTHOLD)
				m_Force.y = m_Jump;
		}

		void Creature::analyze_stance()
		{

			if (m_Foothold != NO_FOOTHOLD)
			{
				if (m_Force.x < -0.1f || m_Force.x > 0.1f)
					change_stance(Walk);
				else if (Stand != m_StanceState)
					change_stance(Stand);
			}
			else
			{
				change_stance(Jump);
			}
		}

		void Creature::draw(graphics::Renderer* renderer) const
		{
			switch (m_Direction)
			{
			case Left:
				renderer->push(m_TransformationMatrix);
				break;
			case Right:
				renderer->push(m_TransformationMatrix*maths::mat4::Translate(maths::vec3(m_Bounds.width / 2, 0))*maths::mat4::Scale(maths::vec3(-1, 1, 1))*maths::mat4::Translate(-maths::vec3(m_Bounds.width / 2, 0)));
				break;
			}
			for (auto & renderable : m_StatesRenderables.at(m_StanceState))
				renderable->draw(renderer, m_Color);
				//renderer->submit(renderable.get(), m_Color);
			renderer->pop();
		}
		
		void Creature::change_stance(StanceState next_state)
		{
			if (m_StanceState != next_state)
				for (auto & rend : m_StatesRenderables[next_state])
					rend->resetState();
			m_StanceState = next_state;
		}

		void Creature::getHit(Direction dir, Damage damage)
		{
			if (!is_hit)
			{
				causeDamage(damage);
				m_Force = calculateForce(maths::vec2(dir * 20, 8));
				//m_Direction = oppositeDirection(dir);
				if (m_IsImmuneAfterHit)
				{
					is_hit = true;
					m_HitTimer.reSet(1.5f);
					m_Color &= 0x80ffffff;
				}
				//m_Color = 0x80ffffff;
			}
		}
	}
}
