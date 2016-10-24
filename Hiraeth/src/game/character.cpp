#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec3 pos, graphics::Texture* body_tex, graphics::Texture* hand_tex, graphics::Texture* head_tex, 
			graphics::Texture* body_walk_tex, graphics::Texture* hand_walk_tex, Timer* time, graphics::Window* wnd)
			//: m_Bounds(maths::vec2(0,0), maths::vec2(0,0)),
			: m_Force(0.0f), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			//graphics::Renderable(pos, 3 ,0.4f, false, ptex,time), 
			m_TransformationMatrix(maths::mat4::Translate(pos)),
			m_Wnd(wnd), m_MovementTimer(time->elapsed()), m_Time(time)
		{
			m_Bounds = maths::Rectangle(pos, maths::vec2(body_tex->getHeight(), body_tex->getWidth()));
			m_Renderables.push_back(new graphics::SpritedRenderable(pos, 3, 0.4f, false, body_tex, time));
			m_Renderables.push_back(new graphics::SpritedRenderable(pos + maths::vec3(18,10,0), 3, 0.4f, false, hand_tex, time));
			m_Renderables.push_back(new graphics::SpritedRenderable(pos + maths::vec3(-4,29,0), 3, 0.4f, false, head_tex, time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(pos, 4, 0.4f, true, body_walk_tex, time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(pos + maths::vec3(11,11,0), 4, 0.4f, true, hand_walk_tex, time));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(pos + maths::vec3(-4,29,0), 3, 0.4f, true, head_tex, time));
		}

		Character::~Character()
		{
			for (int i = 0; i < m_Renderables.size(); i++)
			{
					delete m_Renderables[i];
			}
		}

		void Character::update()
		{
			switch (m_StanceState)
			{
			case StanceState::Stand:
				for (graphics::SpritedRenderable* renderable : m_Renderables)
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
					//m_TransformationMatrix *= maths::mat4::translation(maths::vec3(CHARACTER_SPEED, 0,0));
					//move(maths::vec2(CHARACTER_SPEED, 0));
					//m_StanceState = StanceState::Walk;
					m_Direction = Direction::Right;
					m_Force.x += CHARACTER_SPEED;
				}
				if (m_Wnd->isKeyPressed(GLFW_KEY_A))
				{
					m_Force.x -= CHARACTER_SPEED;
					m_Direction = Direction::Left;
				}
				if (m_Wnd->isKeyPressed(GLFW_KEY_W))
					m_Force.y += CHARACTER_SPEED;
				//else
				//	m_Force.y -= CHARACTER_GRAVITY;
				if (m_Wnd->isKeyPressed(GLFW_KEY_S))
					m_Force.y -= CHARACTER_SPEED;
				if (maths::Rectangle(-800, -450, 1600, 345).Intersects(m_Bounds + maths::vec2(m_Force.x,0)))
				{
					//std::cout << "it intersects. pos : " << m_Bounds.position << "size : " << m_Bounds.size << std::endl;
					m_Force.x = 0;
				}
				if (maths::Rectangle(-800, -450, 1600, 345).Intersects(m_Bounds + maths::vec2(0, m_Force.y)))
				{
					//std::cout << "it intersects. pos : " << m_Bounds.position << "size : " << m_Bounds.size << std::endl;
					m_Force.y = 0;
				}
				m_MovementTimer = m_Time->elapsed();
				m_Force *= FRICTION;
				move(m_Force);
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
				//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.GetMinimumBound());
			}
		}
		void Character::submit(graphics::Renderer2D* renderer) const
		{
			switch (m_Direction)
			{
			case Direction::Left:
			renderer->push(m_TransformationMatrix);
				break;
			case Direction::Right:
				renderer->push(m_TransformationMatrix*maths::mat4::Translate(maths::vec3(15,0))*maths::mat4::Scale(maths::vec3(-1, 1, 1))*maths::mat4::Translate(-maths::vec3(15,0)));
				break;
			}
			switch (m_StanceState)
			{
			case StanceState::Stand:
					for (const Renderable2D* renderable : m_Renderables)
						renderable->submit(renderer);
					break;
			case StanceState::Walk:
					for (const Renderable2D* renderable : m_WalkRenderables)
						renderable->submit(renderer);
					break;
			}
			renderer->pop();
		}
	}
}
