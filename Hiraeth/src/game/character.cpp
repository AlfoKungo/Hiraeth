#include "character.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer *map_layer, item::ItemManager *item_manager,
			skills::SkillManager *skill_manager, CharacterStats * character_stats)
			: Creature(maths::Rectangle(pos, maths::vec2(32, 45)), map_layer, character_stats, true),
			m_Kb(kb),
			m_ItemManager(item_manager),
			m_SkillManager(skill_manager),
			m_CharacterStats(character_stats)
		{
			m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterBody>(
				SRL::AnimationData{ {{{0, 0, 23, 31}, {15, 0}, 0.4f}, {{23, 0, 23, 31}, {15, 0}, 0.4f}, {{46, 0, 23, 31}, {15, 0}, 0.4f}}, true },
				graphics::TextureManager::Load("char_body_stand.png")));
			m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterArm>(
				SRL::AnimationData{ {{{0, 0, 11, 19}, {33, 10}, 0.4f}, {{11, 0, 11, 19}, {33, 10}, 0.4f}, {{22, 0, 11, 19}, {33, 10}, 0.4f}}, true },
				graphics::TextureManager::Load("char_hand_stand.png"),
				std::vector<maths::vec2>{maths::vec2(28, 10), maths::vec2(28, 9), maths::vec2(30, 10)}));
			m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
				SRL::AnimationData{ {{{0, 0, 45, 35}, {11, 29}, 0.4f}, {{45, 0, 45, 35}, {11, 29}, 0.4f}, {{90, 0, 45, 35}, {11, 29}, 0.4f}}, true },
				graphics::TextureManager::Load("char_head.png")));

			m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterBody>(
				SRL::AnimationData{ {{{0, 0, 35, 32}, {18, 0}, 0.18f}, {{35, 0, 35, 32}, {18, 0}, 0.18f}, {{70, 0, 35, 32}, {18, 0}, 0.18f}, {{105, 0, 35, 32}, {18, 0}, 0.18f}}, false },
				graphics::TextureManager::Load("char_body_walk.png")));
			m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterArm>(
				SRL::AnimationData{ {{{0, 0, 30, 17}, {29, 11}, 0.18f}, {{30, 0, 30, 17}, {29, 11}, 0.18f},
					{{60, 0, 30, 17}, {29, 11}, 0.18f}, {{90, 0, 30, 17}, {29, 11}, 0.18f}}, false },
				graphics::TextureManager::Load("char_hand_walk.png"), std::vector<maths::vec2>{ maths::vec2(33, 11),
				maths::vec2(22, 10), maths::vec2(33, 11), maths::vec2(35, 12) }));
			m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
				SRL::AnimationData{ {{{0, 0, 45, 35}, {14, 29}, 0.18f}, {{45, 0, 45, 35}, {14, 29}, 0.18f}, {{90, 0, 45, 35}, {14, 29}, 0.18f}}, false },
				graphics::TextureManager::Load("char_head.png")));

			m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterBody>(
				SRL::AnimationData{ {{{0, 0, 45, 33}, {19, 0}, 0.35f}, {{45, 0, 45, 33}, {19, 0}, 0.45f }}, false },
				graphics::TextureManager::Load("char_body_attack.png")));
			m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterArm>(
				SRL::AnimationData{ {{{0, 0, 30, 18}, {49, 10}, 0.35f}, {{30, 0, 30, 18}, {0, 11}, 0.45f}}, false },
				graphics::TextureManager::Load("char_hand_attack.png"), std::vector<maths::vec2>{ maths::vec2(47, 10), maths::vec2(-8, 19)}));
			m_StatesRenderables[Attack].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
				SRL::AnimationData{ {{{0, 0, 45, 35}, {25, 27}, 0.35f}, {{45, 0, 45, 35}, {8, 26}, 0.45f}}, false },
				graphics::TextureManager::Load("char_head_attack.png")));

			m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterBody>(
				SRL::AnimationData{ {{{0, 0, 35, 30}, {17, 0}, 0.4f}}, false },
				graphics::TextureManager::Load("char_body_jump.png")));
			m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterArm>(
				SRL::AnimationData{ {{{0, 0, 13, 11}, {34, 17}, 0.4f}}, false },
				graphics::TextureManager::Load("char_hand_jump.png"), std::vector<maths::vec2>{ maths::vec2{ 28, 18 }}));
			m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 }, SRL::AnimationData{ {{{0, 0, 39, 35}, {18, 28}, 0.4f}}, false }, graphics::TextureManager::Load("char_head_jump.png")));

			m_HitBox = maths::vec2{ 32, 45 };
			m_Org = maths::vec2{ m_HitBox.x / 2, 0 };
			m_HitSprite = graphics::Sprite{ maths::vec2(0, 0), getBounds().width, getBounds().height, 0x600045ff };

			registerKeys();
		}

		Character::~Character()
		{
		}

		void Character::update()
		{
			m_CharacterStats->update();

			m_Speed = m_CharacterStats->getSpeed();
			m_Jump = m_CharacterStats->getJump();
			if (m_Controls.pick_up)
			{
				pickItemUp();
			}
			Creature::update();
		}

		void Character::ButtonReleased(input::Key control)
		{
			ButtonUpdate(control, false);
		}

		void Character::ButtonClicked(input::Key control)
		{
			ButtonUpdate(control, true);
		}

		void Character::ButtonUpdate(input::Key key, bool state)
		{
			if (key == right)
				m_Controls.right = state;
			else if (key == left)
				m_Controls.left = state;
			else if (key == down)
				m_Controls.down = state;
			else if (key == jump)
				m_Controls.jump = state;
			else if (key == attack)
				m_Controls.attack = state;
			else if (key == pick_up)
				m_Controls.pick_up = state;
			else
				if (state)
				{
					unsigned int skill_index = m_SkillKeysMap[key];
					activate_skill(skill_index);
				}
		}

		void Character::registerKeys()
		{
			//m_Kb->registerToKey(GLFW_KEY_UP, up, this);
			m_Kb->registerToKey(GLFW_KEY_DOWN, down, this);
			m_Kb->registerToKey(GLFW_KEY_LEFT, left, this);
			m_Kb->registerToKey(GLFW_KEY_RIGHT, right, this);
			m_Kb->registerToKey(GLFW_KEY_SPACE, jump, this);
			m_Kb->registerToKey(GLFW_KEY_LEFT_CONTROL, attack, this);
			m_Kb->registerToKey(GLFW_KEY_Z, pick_up, this);
			std::vector<unsigned int> skill_keys{ GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B, GLFW_KEY_N };
			std::vector<unsigned int> available_skills = m_SkillManager->get_available_skills();
			for (int i = 0; i < available_skills.size(); ++i)
			{
				m_SkillKeysMap[(pick_up + 1) + i] = available_skills[i];
				m_Kb->registerToKey(skill_keys[i], (pick_up + 1) + i, this);
			}
		}

		void Character::carryOutAttack()
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
				if (monster->check_collision(attack_rec))
				{
					std::cout << "attacking" << std::endl;
					monster->getHit(m_Direction, getDamage());
				}
			}
		}

		void Character::pickItemUp()
		{
			item::Item * item = m_ItemManager->getItem(m_Bounds.GetBottomMiddle());
			if (item != NULL)
				item->pickUp(&getBounds());
		}

		void Character::activate_skill(unsigned skill_index)
		{
			SRL::SkillInfo * skill_info = m_SkillManager->get_skill(skill_index);
			SRL::SkillPropertiesMap * item_properties = &skill_info->skill_properties;
			if (item_properties->find(SRL::SkillDataType::mpCon) != item_properties->end())
				if (!m_Stats->consumeMana(std::get<int>(item_properties->at(SRL::SkillDataType::mpCon))))
					return;
			for (SRL::SkillPropertiesMapPair element : (*item_properties))
			{
				switch (element.first)
				{
				case SRL::SkillDataType::dmg:
					m_Stats->causeDamage(Damage{ unsigned int(std::get<int>(element.second)) });
					break;
				case SRL::SkillDataType::heal:
					m_Stats->recoverHp(std::get<int>(element.second));
					break;
				case SRL::SkillDataType::speed:
					//m_CharacterStats->setTimedStat(std::get<SRL::TimedValue>(element.second));
					break;
				case SRL::SkillDataType::dmgS:
					m_Stats->causeDamage(Damage{ 25, 25 });
					break;
				default:
					break;
				}
			}
		}
	}
}