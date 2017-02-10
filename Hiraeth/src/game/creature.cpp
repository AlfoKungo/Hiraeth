#include "creature.h"

namespace hiraeth {
	namespace game {

		Creature::Creature(maths::vec3 pos, Timer* time, input::Keyboard* kb, 
			map::MapLayer* mapLayer)
			: Collisionable(&m_Bounds, mapLayer),
			m_Force(0.0f), m_StanceState(StanceState::Stand), m_Direction(Direction::Left),
			//m_TransformationMatrix(maths::mat4::Translate(pos)), m_Foothold(-1),
			//m_Wnd(wnd), m_MovementTimer(time->elapsed()), m_Time(time), m_MapLayer(m_MapLayer)
			m_TransformationMatrix(maths::mat4::Translate(pos)), 
			m_Kb(kb), m_MovementTimer(time->elapsed()), m_Time(time)
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

			if (m_Time->elapsed() - m_MovementTimer > CHARACTER_TIME_BETWEEN_ADDS)
			{
				analyze_controls();

				add_gravity(&m_Force.y);
				m_Force *= FRICTION;
				std::string d = "pos: " + m_Bounds.position.ToString() + " ; force: " + m_Force.ToString();
				//m_Wnd->setTitle(d.c_str());
				physics::CollisionStruct FootHold = analyze_collision(m_Bounds, m_Force);
				//turn this to x value alignment
				if (FootHold.y != NO_FOOTHOLD)
					m_Force.x = 0;
				if (m_Foothold == NO_FOOTHOLD)
				{
					if (FootHold.x != NO_FOOTHOLD)
					{
						m_Force.y = 0;
						set_foothold(FootHold.x);
						move_to(set_y_by_foothold(m_Force));
					}
				}
				else
				{
					if (!check_if_still_on_foothold())
						m_Foothold = NO_FOOTHOLD;
					else if (m_Force.y > 0)
						m_Foothold = NO_FOOTHOLD;
					else
						move_to(set_y_by_foothold(m_Force));
				}
				move(m_Force);
				m_TransformationMatrix *= maths::mat4::Translate(m_Force);
				//m_TransformationMatrix = m_TransformationMatrix.Translate(m_Bounds.position);

				m_MovementTimer = m_Time->elapsed();
			}
		}

		void Creature::analyze_controls()
		{
			CreatureControls controls = set_update_controls();
			if (controls.right)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Right;
				m_Force.x += CHARACTER_SPEED;
			}
			else if (controls.left)
			{
				change_stance(StanceState::Walk);
				m_Direction = Direction::Left;
				m_Force.x -= CHARACTER_SPEED;
			}
			else
			{
				change_stance(StanceState::Stand);
			}
			if (controls.up)
				m_Force.y += CHARACTER_SPEED;
			if (controls.down)
				m_Force.y -= CHARACTER_SPEED;
			if (m_Kb->isKeyPressed(GLFW_KEY_SPACE))
			{
				m_Force.y += 3.0f;
			}

		}

		//void Creature::set_foothold(int foothold_index)
		//{
		//	m_Foothold = foothold_index;
		//	set_y_by_foothold();
		//}

		//bool Creature::check_if_still_on_foothold()
		//{
		//	physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
		//	return (m_Bounds.GetMaximumBound().x > foothold.p1.x && m_Bounds.GetMinimumBound().x < foothold.p2.x);
		//}

		//void Creature::set_y_by_foothold()
		//{
		//	m_Force.y = 0;
		//	physics::FootHold foothold = m_MapLayer->m_FootHolds.at(m_Foothold);
		//	float d = 0, y_pos;
		//	if (foothold.p1.x != foothold.p2.x)
		//		d = (foothold.p1.x - m_Bounds.position.x) / (foothold.p1.x - foothold.p2.x);
		//	y_pos = d * foothold.p2.y + (1 - d) * foothold.p1.y;
		//	move_to(maths::vec2(m_Bounds.position.x, y_pos));
		//}

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

		//physics::CollisionStruct Creature::analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed)
		//{
		//	std::vector<physics::FootHold> *m_FootHolds = &m_MapLayer->m_FootHolds;
		//	physics::CollisionStruct collision = { NO_FOOTHOLD, NO_FOOTHOLD };
		//	float x_force = char_speed.x, y_force = char_speed.y;
		//	maths::Rectangle next_char_rec = char_rec;
		//	next_char_rec.position += char_speed;
		//	
		//	for (int i = 0; i < m_FootHolds->size(); i++)
		//	{
		//		if (m_FootHolds->at(i).LinesIntersect(char_rec.position, char_rec.position + char_speed)
		//		//if (m_FootHolds->at(i).Intersects(next_char_rec) && !m_FootHolds->at(i).Intersects(char_rec)
		//			&& m_FootHolds->at(i).is_solid() && y_force < 0)
		//		{
		//		collision.x = i;
		//		}
		//		if (m_FootHolds->at(i).Intersects(next_char_rec)
		//			&& !m_FootHolds->at(i).is_solid())
		//		{
		//			collision.y = i;
		//		}
		//	}
		//	return collision;
		//}
	}
}
