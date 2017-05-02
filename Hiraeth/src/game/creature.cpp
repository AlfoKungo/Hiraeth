#include "creature.h"

namespace hiraeth {
	namespace game {

		Creature::Creature(maths::vec3 pos, Timer* time, input::Keyboard* kb, 
			map::MapLayer* mapLayer)
			: Collisionable(m_Bounds, mapLayer),
			m_Force(0.0f), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			m_TransformationMatrix(maths::mat4::Translate(pos)), 
			m_MovementTimer(time->elapsed()), m_Time(time)
		{
			m_Bounds = maths::Rectangle(pos, maths::vec2(graphics::TextureManager::Load("char_head.png")->getWidth()/3, graphics::TextureManager::Load("char_body.png")->getHeight()));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.4f, false, graphics::TextureManager::Load("char_body.png"), time));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(18,10), 3, 0.4f, false, graphics::TextureManager::Load("char_hand.png"), time));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, false, graphics::TextureManager::Load("char_head.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 4, 0.4f, true, graphics::TextureManager::Load("char_body_walk.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(11,11), 4, 0.4f, true, graphics::TextureManager::Load("char_hand_walk.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, true, graphics::TextureManager::Load("char_head.png"), time));
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

		inline void Creature::move_to(const maths::vec2& pos)
		{
			m_Bounds.position = pos;
			m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);
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

			//if (m_Time->elapsed() - m_MovementTimer > CHARACTER_TIME_BETWEEN_ADDS)
			//{
				analyze_controls();
				//m_Force *= (m_Time->elapsed() - m_MovementTimer)*60;

				m_LastFoothold = m_Foothold;
				add_gravity(&m_Force.y);
				m_Force *= FRICTION;
				//std::string d = "pos: " + m_Bounds.position.ToString() + " ; force: " + m_Force.ToString();
				//m_Wnd->setTitle(d.c_str());
				physics::CollisionStruct FootHold = analyze_collision(m_Bounds, m_Force);
				if (FootHold.y != NO_FOOTHOLD)
				{
					m_Force = force_by_vertical_foothold(m_Force, FootHold.y);
				}
				if (m_Foothold == NO_FOOTHOLD)
				{
					if (FootHold.x != NO_FOOTHOLD)
					{
						set_foothold(FootHold.x);
						m_Force = set_y_by_foothold(m_Force);
					}
				}
				else
				{
					//if (m_Force.y > 0)
					if (m_Controls.jump)
						m_Foothold = NO_FOOTHOLD;
					else if (!check_if_still_on_foothold())
						m_Foothold = NO_FOOTHOLD;
					else
					{
						m_Force = set_y_by_foothold(m_Force);
					}
				}
				move(m_Force);
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
				if (m_Foothold != NO_FOOTHOLD)
				{
					m_Force.y = 0;
				}

				//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);

				m_MovementTimer = m_Time->elapsed();
			//}
		}

		void Creature::analyze_controls()
		{
			//m_Controls = set_update_controls();
			if (m_Controls.right)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Right;
				m_Force.x += CHARACTER_SPEED;
			}
			else if (m_Controls.left)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Left;
				m_Force.x -= CHARACTER_SPEED;
			}
			else
			{
				change_stance(StanceState::Stand);
			}
			if (m_Controls.up)
				m_Force.y += CHARACTER_SPEED;
			if (m_Controls.down)
				m_Force.y -= CHARACTER_SPEED;
			if (m_Controls.jump && m_Foothold != NO_FOOTHOLD) 
				m_Force.y += CHARACTER_JUMP;
		}

		void Creature::submit(graphics::Renderer2D* renderer) const
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
				for (const Renderable2D* renderable : m_StandRenderables)
					renderable->submit(renderer);
				break;
			case StanceState::Walk:
				for (const Renderable2D* renderable : m_WalkRenderables)
					renderable->submit(renderer);
				break;
			}
			renderer->pop();
		}
		
		void Creature::change_stance(StanceState next_state)
		{
			m_StanceState = next_state;
		}

	}
}
