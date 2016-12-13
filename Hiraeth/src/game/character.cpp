#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, Timer* time, graphics::Window* wnd, 
			map::MapLayer* m_MapLayer)
			: m_Force(0.0f), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			//graphics::Renderable(pos, 3 ,0.4f, false, ptex,time), 
			m_TransformationMatrix(maths::mat4::Translate(pos)), m_Foothold(-1),
			m_Wnd(wnd), m_MovementTimer(time->elapsed()), m_Time(time), m_MapLayer(m_MapLayer)
		{
			m_Bounds = maths::Rectangle(pos, maths::vec2(graphics::TextureManager::Load("char_head.png")->getWidth()/3, graphics::TextureManager::Load("char_body.png")->getHeight()));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.4f, false, graphics::TextureManager::Load("char_body.png"), time));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(18,10), 3, 0.4f, false, graphics::TextureManager::Load("char_hand.png"), time));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, false, graphics::TextureManager::Load("char_head.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 4, 0.4f, true, graphics::TextureManager::Load("char_body_walk.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(11,11), 4, 0.4f, true, graphics::TextureManager::Load("char_hand_walk.png"), time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, true, graphics::TextureManager::Load("char_head.png"), time));
		}

		Character::~Character()
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

		inline void Character::move_to(const maths::vec2& pos)
		{
			m_Bounds.position = pos;
			m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);
		}

		void Character::update()
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

			if (m_Time->elapsed() - m_MovementTimer > CHARACTER_TIME_BETWEEN_ADDS)
			{
				if (m_Wnd->isKeyPressed(GLFW_KEY_D))
				{
					change_stance(StanceState::Walk);
					m_Direction = Direction::Right;
					m_Force.x += CHARACTER_SPEED;
				}
				else if (m_Wnd->isKeyPressed(GLFW_KEY_A))
				{
					change_stance(StanceState::Walk);
					m_Direction = Direction::Left;
					m_Force.x -= CHARACTER_SPEED;
				}
				else
				{
					change_stance(StanceState::Stand);
				}
				if (m_Wnd->isKeyPressed(GLFW_KEY_W))
					m_Force.y += CHARACTER_SPEED;
				if (m_Wnd->isKeyPressed(GLFW_KEY_S))
					m_Force.y -= CHARACTER_SPEED;
				if (m_Wnd->isKeyTyped(GLFW_KEY_SPACE))
				{
					m_Force.y += 3.0f;
				}
				m_MovementTimer = m_Time->elapsed();
				//m_Force.y -= 0.5f;
				m_Force *= FRICTION;
				if (m_Foothold == NO_FOOTHOLD)
				{
					int FootHold = analyze_collision(m_Bounds, m_Force);
					if (FootHold != NO_FOOTHOLD)
						set_foothold(FootHold);
				}
				else
				{
					if (!check_if_still_on_foothold())
						m_Foothold = NO_FOOTHOLD;
					else if (m_Force.y > 0)
						m_Foothold = NO_FOOTHOLD;
					else
						set_y_by_foothold();
				}
				move(m_Force);
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
				//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);
			}
		}

		void Character::set_foothold(int foothold_index)
		{
			m_Foothold = foothold_index;
			set_y_by_foothold();
		}

		bool Character::check_if_still_on_foothold()
		{
			physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
			return (m_Bounds.GetMaximumBound().x > foothold.p1.x && m_Bounds.GetMinimumBound().x < foothold.p2.x);
		}

		void Character::set_y_by_foothold()
		{
			m_Force.y = 0;
			physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
			float d = 0, y_pos;
			if (foothold.p1.x != foothold.p2.x)
				d = (foothold.p1.x - m_Bounds.position.x) / (foothold.p1.x - foothold.p2.x);
			y_pos = d * foothold.p2.y + (1 - d) * foothold.p1.y;
			move_to(maths::vec2(m_Bounds.position.x, y_pos));
		}

		void Character::submit(graphics::Renderer2D* renderer) const
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
		
		void Character::change_stance(StanceState next_state)
		{
			m_StanceState = next_state;
		}

		int Character::analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed)
		{
			std::vector<physics::FootHold> *m_FootHolds = &m_MapLayer->m_FootHolds;
			float x_force = char_speed.x, y_force = char_speed.y;
			maths::Rectangle next_char_rec = char_rec;
			next_char_rec.position += char_speed;
			
			for (int i = 0; i < m_FootHolds->size(); i++)
			{
				if (m_FootHolds->at(i).Intersects(next_char_rec) && !m_FootHolds->at(i).Intersects(char_rec)
					&& m_FootHolds->at(i).direction == maths::vec2(0,1) && y_force < 0)
				{
					return i;
					//if (m_FootHolds->at(i).direction.x == 1)
					//	x_force = 0;
					//else
					//	y_force = 0;
					//std::cout << char_rec.position << ", " << char_rec.size << std::endl;
				}
			}
			//return maths::vec2(x_force, y_force);
			return NO_FOOTHOLD;
		}
	}
}
