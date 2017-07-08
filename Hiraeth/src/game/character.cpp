#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer, item::ItemManager *item_manager, CharacterStats * character_stats)
			: Creature(maths::Rectangle(pos, maths::vec2(32, 35)), map_layer, character_stats, true),
			m_Kb(kb),
			m_ItemManager(item_manager)
		{
			m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterBody>(3, 0.4f, false, graphics::TextureManager::Load("char_body_stand.png"), maths::vec2(0)));
			m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterArm>(3, 0.4f, false, graphics::TextureManager::Load("char_hand_stand.png"),
				std::vector<character::ArmConfigs>{character::ArmConfigs{ maths::vec2(18,10),  maths::vec2(14,10) }, character::ArmConfigs{ maths::vec2(18,10),
				maths::vec2(15,10) }, character::ArmConfigs{ maths::vec2(18,10),  maths::vec2(14,10) } }));
			m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(0), 3, 0.4f, false, graphics::TextureManager::Load("char_head.png"), maths::vec2(-4, 29)));
			m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterBody>(4, 0.4f, true, graphics::TextureManager::Load("char_body_walk.png"), maths::vec2(0)));
			m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterArm>(4, 0.4f, true, graphics::TextureManager::Load("char_hand_walk.png"), std::vector<maths::vec2>{ maths::vec2(11, 11), maths::vec2(11, 11), maths::vec2(11, 11), maths::vec2(11, 11) }));
			m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(0), 3, 0.4f, true, graphics::TextureManager::Load("char_head.png"), maths::vec2(-4, 29)));
			m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterBody>(2, std::vector<float>{0.1f, 0.3f}, true, graphics::TextureManager::Load("char_body_attack.png"), maths::vec2(0)));
			m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterArm>(2, std::vector<float>{0.1f, 0.3f}, true, graphics::TextureManager::Load("char_hand_attack.png"), std::vector<maths::vec2>{ maths::vec2(30, 10), maths::vec2(-19, 11)}));
			m_StatesRenderables[Attack].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(0), 2, std::vector<float>{0.1f, 0.3f}, true, graphics::TextureManager::Load("char_head_attack.png"), std::vector<maths::vec2>{ maths::vec2(6, 27), maths::vec2(-11, 26)}));
			m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterBody>(1, 0.4f, false, graphics::TextureManager::Load("char_body_jump.png"), maths::vec2(0)));
			m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterArm>(1, 0.4f, false, graphics::TextureManager::Load("char_hand_jump.png"), std::vector<maths::vec2>{ maths::vec2(17, 17)}));
			m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(0), 1, 0.4f, false, graphics::TextureManager::Load("char_head_jump.png"), std::vector<maths::vec2>{ maths::vec2(1, 28)}));
			registerKeys();
		}

		Character::~Character()
		{
		}

		void Character::update()
		{
			if (m_Controls.pick_up)
			{
				pickItemUp();
			}
			Creature::update();
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
			if (control == input::Controls::pick_up)
				m_Controls.pick_up = false;
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
			if (control == input::Controls::pick_up)
				m_Controls.pick_up = true;
		}

		void Character::registerKeys()
		{
			//m_Kb->registerToKey(input::Controls::up, this);
			m_Kb->registerToKey(input::Controls::down, this);
			m_Kb->registerToKey(input::Controls::right, this);
			m_Kb->registerToKey(input::Controls::left, this);
			m_Kb->registerToKey(input::Controls::jump, this);
			m_Kb->registerToKey(input::Controls::attack, this);
			m_Kb->registerToKey(input::Controls::pick_up, this);
		}

		void Character::attack()
		{
			maths::Rectangle attack_rec = getBounds();
			if (Left == m_Direction)
			{
				attack_rec.x = attack_rec.x - (85 - attack_rec.width);
			}
			attack_rec.width = 85;
			attack_rec.position += m_Direction * maths::vec2(15);
			for (auto monster : (*m_MonstersLayer))
			{
				if (monster->checkCollision(attack_rec))
				{
					std::cout << "attacking" << std::endl;
					monster->getHit(m_Direction, getDamage());
				}
			}
		}

		//CharacterStats* Character::getCharacterStats() const
		//{
		//	return static_cast<CharacterStats*>(m_Stats.get());
		//}

		void Character::pickItemUp()
		{
			item::Item * item = m_ItemManager->getItem(m_Bounds.GetBottomMiddle());
			if (item != NULL)
				item->pickUp(&getBounds());
		}
	}
}