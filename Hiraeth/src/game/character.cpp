#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb,
			map::MapLayer* m_MapLayer)
			: Creature(maths::Rectangle(pos, maths::vec2(45,31)), m_MapLayer),
			m_Kb(kb)
		{
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 3, 0.4f, false, graphics::TextureManager::Load("char_body.png")));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(18,10), 3, 0.4f, false, graphics::TextureManager::Load("char_hand.png")));
			m_StandRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, false, graphics::TextureManager::Load("char_head.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2(), 4, 0.4f, true, graphics::TextureManager::Load("char_body_walk.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(11,11), 4, 0.4f, true, graphics::TextureManager::Load("char_hand_walk.png")));
			m_WalkRenderables.push_back(new graphics::SpritedRenderable(maths::vec2() + maths::vec2(-4,29), 3, 0.4f, true, graphics::TextureManager::Load("char_head.png")));
			registerKeys();
		}

		Character::~Character()
		{

		}

		void Character::ButtonReleased(input::Controls control)
		{
			if (control == input::Controls::right)
				m_Controls.right = false;
			else if (control == input::Controls::left)
				m_Controls.left = false;
			if (control == input::Controls::down)
				m_Controls.down = false;
			if (control == input::Controls::jump)
				m_Controls.jump = false;
		}

		void Character::ButtonClicked(input::Controls control)
		{
			if (control == input::Controls::right)
				m_Controls.right = true;
			else if (control == input::Controls::left)
				m_Controls.left = true;
			if (control == input::Controls::down)
				m_Controls.down = true;
			if (control == input::Controls::jump)
				m_Controls.jump = true;
		}
		
		void Character::update()
		{
			if (is_hit && StaticTimer::timer.elapsed() - m_HitTimer > 0.0f)
				is_hit = false;
			Creature::update();
		}

		void Character::registerKeys()
		{
			m_Kb->registerToKey(input::Controls::up, this);
			m_Kb->registerToKey(input::Controls::down, this);
			m_Kb->registerToKey(input::Controls::right, this);
			m_Kb->registerToKey(input::Controls::left, this);
			m_Kb->registerToKey(input::Controls::jump, this);

		}

		void Character::getHit(Direction dir)
		{
			if (Direction::Left == dir)
			{
				m_Force = maths::vec2(20, 8);
					m_Direction = Direction::Left;
			}
			else
			{
				m_Force = maths::vec2(-20, 8);
					m_Direction = Direction::Right;
			}
			is_hit = true;
			m_HitTimer = StaticTimer::timer.elapsed() + 1.5f;
		}
	}
}
