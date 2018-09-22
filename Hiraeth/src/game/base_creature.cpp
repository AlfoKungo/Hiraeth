#include "base_creature.h"

namespace hiraeth
{
	namespace game
	{
		BaseCreature::BaseCreature(maths::Rectangle bounds, map::MapLayer* map_layer, 
			std::unique_ptr<float> speed, std::unique_ptr<float> jump,
			unsigned int foothold)
			: Collisionable(m_Bounds, map_layer, foothold),
			  m_StanceState(Stand), m_Direction(Left), m_CurrentRenderables(nullptr),
			  m_TransformationMatrix(maths::mat4::Translate(bounds.position)), 
			  m_Speed(std::move(speed)), m_Jump(std::move(jump))
		{
			m_StatesRenderables[Stand] = std::vector<std::unique_ptr<Renderable>>();
			m_StatesRenderables[Walk] = std::vector<std::unique_ptr<Renderable>>();
			m_StatesRenderables[Attack] = std::vector<std::unique_ptr<Renderable>>();
			m_StatesRenderables[Jump] = std::vector<std::unique_ptr<Renderable>>();
			m_Bounds = bounds;
		}

		void BaseCreature::update()
		{
			for (auto& renderable : m_StatesRenderables.at(m_StanceState))
				renderable->update();

			analyze_controls();
			add_gravity(m_Force);
			multiply_by_friction(m_Force);
			analyze_collision();
		}

		void BaseCreature::draw(graphics::Renderer* renderer) const
		{
			if (m_Direction == Left)
				renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)));
			else
				//renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)) * 
				//	maths::mat4::Scale(maths::vec3(-1, 1, 1)));
				renderer->push(m_TransformationMatrix * maths::mat4::Translate(maths::vec3(m_Org)) * 
					maths::mat4::Scale(maths::vec3(-1, 1, 1)) * maths::mat4::Translate(- maths::vec3(2 * m_Org)));

			for (auto& renderable : m_StatesRenderables.at(m_StanceState))
				renderable->draw(renderer, m_Color);
			renderer->pop();
		}

		void BaseCreature::analyze_controls()
		{
			if (m_Controls.right)
			{
				m_Direction = Right;
				m_Force.x += calculate_force(*m_Speed);
			}
			else if (m_Controls.left)
			{
				m_Direction = Left;
				m_Force.x -= calculate_force(*m_Speed);
			}
			if (m_Controls.jump && m_Foothold != NO_FOOTHOLD)
				m_Force.y = *m_Jump;
		}

		void BaseCreature::analyze_stance()
		{
			if (Attack != m_StanceState)
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
		}

		void BaseCreature::change_stance(StanceState next_state)
		{
			if (m_StanceState != next_state)
			{
				m_StanceState = next_state;
				for (auto& rend : m_StatesRenderables[next_state])
					rend->resetState();
			}
		}


	}
}
