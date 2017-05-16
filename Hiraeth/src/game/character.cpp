#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer)
			: Creature(maths::Rectangle(pos, maths::vec2(45,31)), map_layer, CHARACTER_SPEED, CHARACTER_JUMP, 
				new CharacterStats()),
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

		void Character::registerKeys()
		{
			m_Kb->registerToKey(input::Controls::up, this);
			m_Kb->registerToKey(input::Controls::down, this);
			m_Kb->registerToKey(input::Controls::right, this);
			m_Kb->registerToKey(input::Controls::left, this);
			m_Kb->registerToKey(input::Controls::jump, this);
		}

		CharacterStats* Character::getCharacterStats() const
		{
			return static_cast<CharacterStats*>(m_Stats);
		}
	}
}
