#include "creature.h"

namespace hiraeth {
	namespace game {

		Creature::Creature(maths::Rectangle bounds, map::MapLayer* mapLayer, float speed, float jump,
			Stats* stats)
			: Collisionable(m_Bounds, mapLayer), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			m_Stats(stats),
			m_Speed(speed), m_Jump(jump),
			m_HitTimer(StaticTimer::timer.elapsed()), 
			m_MovementTimer(StaticTimer::timer.elapsed()),
		m_TransformationMatrix(maths::mat4::Translate(bounds.position))
		{
			m_Bounds = bounds;
		}

		Creature::~Creature()
		{
			for (int i = 0; i < m_StandRenderables.size(); i++)
			{
					delete m_StandRenderables[i];
			}
			for (int i = 0; i < m_WalkRenderables.size(); i++)
			{
					delete m_WalkRenderables[i];
			}
		}

		void Creature::update()
		{

			switch (m_StanceState)
			{
			case StanceState::Stand:
				for (graphics::SpritedRenderable* renderable : m_StandRenderables)
					renderable->update();
				break;
			case StanceState::Walk:
				for (graphics::SpritedRenderable* renderable : m_WalkRenderables)
					renderable->update();
				break;
			}

			if (is_hit)
				if (StaticTimer::timer.elapsed() - m_HitTimer > 0.0f)
				{
					is_hit = false;
					m_Color = 0xffffffff;
				}
				else
				{
					m_Color = 0x00ffffff | ((unsigned char((m_HitTimer - StaticTimer::timer.elapsed()) / 1.5f * 128 * 2) % 128 + 86) << 24);
				}

			//if (m_Time->elapsed() - m_MovementTimer > CHARACTER_TIME_BETWEEN_ADDS)
			//{
			analyze_controls();
			//m_Force *= (m_Time->elapsed() - m_MovementTimer)*60;
			addGravity(m_Force);
			multiplyByFriction(m_Force);
			analyzeCollision();

			//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);

			//m_MovementTimer = StaticTimer::timer.elapsed();
			//}
		}

		void Creature::analyze_controls()
		{
			if (m_Controls.right)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Right;
				m_Force.x += calculateForce(m_Speed);
			}
			else if (m_Controls.left)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Left;
				m_Force.x -= calculateForce(m_Speed);
			}
			else
			{
				change_stance(StanceState::Stand);
			}
			if (m_Controls.down)
				m_Force.y -= m_Speed;
			if (m_Controls.jump && m_Foothold != NO_FOOTHOLD)
				m_Force.y += m_Jump;
		}

		void Creature::draw(graphics::Renderer2D* renderer) const
		{
			switch (m_Direction)
			{
			case Direction::Left:
				renderer->push(m_TransformationMatrix);
				break;
			case Direction::Right:
				renderer->push(m_TransformationMatrix*maths::mat4::Translate(maths::vec3(15, 0))*maths::mat4::Scale(maths::vec3(-1, 1, 1))*maths::mat4::Translate(-maths::vec3(15, 0)));
				break;
			}
			switch (m_StanceState)
			{
			case StanceState::Stand:
				for (Renderable2D* renderable : m_StandRenderables)
					renderer->submit(renderable, m_Color);
					//renderable->draw(renderer, m_Color);
				break;
			case StanceState::Walk:
				for (Renderable2D* renderable : m_WalkRenderables)
					renderer->submit(renderable, m_Color);
					//renderable->draw(renderer, m_Color);
				break;
			}
			renderer->pop();
		}
		
		void Creature::change_stance(StanceState next_state)
		{
			m_StanceState = next_state;
		}

		void Creature::getHit(Direction dir, Damage damage)
		{
			m_Stats->causeDamage(damage);
			if (Direction::Left == dir)
				m_Force = calculateForce(maths::vec2(20, 8));
			else
				m_Force = calculateForce(maths::vec2(-20, 8));
			m_Direction = dir;
			is_hit = true;
			m_HitTimer = StaticTimer::timer.elapsed() + 1.5f;
			m_Color &= 0x80ffffff;
			//m_Color = 0x80ffffff;
		}
	}
}
