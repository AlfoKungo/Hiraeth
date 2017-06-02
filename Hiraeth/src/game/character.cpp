#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer)
			: Creature(maths::Rectangle(pos, maths::vec2(32,35)), map_layer, CHARACTER_SPEED, CHARACTER_JUMP, 
				new CharacterStats()),
			m_Kb(kb)
		{
			m_StatesRenderables[Stand]->push_back(new character::CharacterBody(3, 0.4f, false, graphics::TextureManager::Load("char_body_stand.png"), maths::vec2(0)));
			m_StatesRenderables[Stand]->push_back(new character::CharacterArm(3, 0.4f, false, graphics::TextureManager::Load("char_hand_stand.png"), new maths::vec2[4]{ maths::vec2(18,10), maths::vec2(18,10), maths::vec2(18,10) }));
			m_StatesRenderables[Stand]->push_back(new graphics::SpritedRenderable(maths::vec2(0), 3, 0.4f, false, graphics::TextureManager::Load("char_head.png"), maths::vec2(-4,29)));
			m_StatesRenderables[Walk]->push_back(new character::CharacterBody(4, 0.4f, true, graphics::TextureManager::Load("char_body_walk.png"), maths::vec2(0)));
			m_StatesRenderables[Walk]->push_back(new character::CharacterArm(4, 0.4f, true, graphics::TextureManager::Load("char_hand_walk.png"), new maths::vec2[4]{ maths::vec2(11,11), maths::vec2(11,11), maths::vec2(11,11), maths::vec2(11,11) }));
			m_StatesRenderables[Walk]->push_back(new graphics::SpritedRenderable(maths::vec2(0), 3, 0.4f, true, graphics::TextureManager::Load("char_head.png"), maths::vec2(-4,29)));
			m_StatesRenderables[Attack]->push_back(new character::CharacterBody(2, 0.4f, true, graphics::TextureManager::Load("char_body_attack.png"), maths::vec2(0)));
			m_StatesRenderables[Attack]->push_back(new character::CharacterArm(2, 0.4f, true, graphics::TextureManager::Load("char_hand_attack.png"), new maths::vec2[2]{ maths::vec2(30, 10), maths::vec2(-19, 11)}));
			m_StatesRenderables[Attack]->push_back(new graphics::SpritedRenderable(maths::vec2(0), 2, 0.4f, true, graphics::TextureManager::Load("char_head_attack.png"), new maths::vec2[2]{ maths::vec2(6, 27), maths::vec2(-9, 26)}));
			m_StatesRenderables[Jump]->push_back(new character::CharacterBody(1, 0.4f, false, graphics::TextureManager::Load("char_body_jump.png"), maths::vec2(0)));
			m_StatesRenderables[Jump]->push_back(new character::CharacterArm(1, 0.4f, false, graphics::TextureManager::Load("char_hand_jump.png"), new maths::vec2[1]{ maths::vec2(17, 17)}));
			m_StatesRenderables[Jump]->push_back(new graphics::SpritedRenderable(maths::vec2(0), 1, 0.4f, false, graphics::TextureManager::Load("char_head_jump.png"), new maths::vec2[1]{ maths::vec2(1, 28)}));
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
			if (control == input::Controls::attack)
				m_Controls.attack = false;
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
			if (control == input::Controls::attack)
				m_Controls.attack = true;
		}

		void Character::registerKeys()
		{
			m_Kb->registerToKey(input::Controls::up, this);
			m_Kb->registerToKey(input::Controls::down, this);
			m_Kb->registerToKey(input::Controls::right, this);
			m_Kb->registerToKey(input::Controls::left, this);
			m_Kb->registerToKey(input::Controls::jump, this);
			m_Kb->registerToKey(input::Controls::attack, this);
		}

		void Character::attack()
		{
			for (auto monster : (*m_MonstersLayer))
			{
				if (monster->checkCollision(getBounds() + (m_Direction * maths::vec2(20, 0))))
				{
					std::cout << "attacking" << std::endl;
					if (getBounds().GetBottomMiddle().x < monster->getBounds().GetBottomMiddle().x)
						monster->getHit(game::Direction::Left, getDamage());
					else
						monster->getHit(game::Direction::Right, getDamage());
				}
			}
		}

		CharacterStats* Character::getCharacterStats() const
		{
			return static_cast<CharacterStats*>(m_Stats);
		}
	}
}