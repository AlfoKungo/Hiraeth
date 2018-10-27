#include "net_char.h"

namespace hiraeth {
	namespace game {

		NetChar::NetChar(maths::vec2 pos, map::MapLayer *map_layer,
			skills::SkillManager *skill_manager)
			: Creature(maths::Rectangle(pos, maths::vec2(32, 45)), map_layer, new CharacterStats{}, true),
			m_SkillManager(skill_manager)
		//m_Animations(m_TransformationMatrix)
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
			m_HitSprite = graphics::Sprite{ maths::vec2(0, 0), getBounds().width, 
				getBounds().height, 0x600045ff };

			//registerKeys();
		}

		NetChar::~NetChar()
		{
		}

		void NetChar::update()
		{
			//m_CharacterStats->update();
			if (m_Animation)
			{
				m_Animation->update();
				if (m_Animation->hasSpriteFinished())
					m_Animation.reset(nullptr);
			}
			m_Animations.update();
			m_Animations.m_Renderables.erase(
				std::remove_if(begin(m_Animations.m_Renderables), end(m_Animations.m_Renderables),
					[](const auto& sr) {return sr->hasSpriteFinished(); }), end(m_Animations.m_Renderables));

			//m_Speed = m_CharacterStats->getSpeed();
			//m_Jump = m_CharacterStats->getJump();
			//if (m_Controls.pick_up)
			//{
			//	pickItemUp();
			//}

			// delete finished timers in m_SkillTimeouts
			auto iter = m_SkillsTimeouts.begin();
			auto endIter = m_SkillsTimeouts.end();
			for (; iter != endIter; )
			{
				if (iter->second.hasExpired())
					m_SkillsTimeouts.erase(iter++);
				else
					++iter;
			}
			Creature::update();
		}

		void NetChar::ButtonUpdate(input::Key key, bool state)
		{
			if (!m_IsStuck)
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
						unsigned int skill_index = m_SkillKeysMap[key - controls_size];
						activateSkill(skill_index);
					}
			}
		}

		//void NetChar::registerKeys()
		//{
		//	//m_Kb->registerToKey(GLFW_KEY_UP, up, this);
		//	m_Kb->registerToKey(GLFW_KEY_DOWN, down, this);
		//	m_Kb->registerToKey(GLFW_KEY_LEFT, left, this);
		//	m_Kb->registerToKey(GLFW_KEY_RIGHT, right, this);
		//	m_Kb->registerToKey(GLFW_KEY_SPACE, jump, this);
		//	m_Kb->registerToKey(GLFW_KEY_LEFT_CONTROL, attack, this);
		//	m_Kb->registerToKey(GLFW_KEY_Z, pick_up, this);
		//	std::vector<unsigned int> skill_keys{ GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_X, GLFW_KEY_B, GLFW_KEY_N };
		//	std::vector<unsigned int> available_skills = m_SkillManager->get_available_skills();
		//	for (int i = 0; i < available_skills.size(); ++i)
		//	{
		//		m_SkillKeysMap[i] = available_skills[i];
		//		m_SkillManager->m_UiSkills->transferSkillToKeyConfig(skill_keys[i], 
		//			(controls_size)+i, available_skills[i], this);
		//	}
		//}

		void NetChar::carryOutAttack()
		{
			//maths::Rectangle attack_rec = getBounds();
			//if (Left == m_Direction)
			//{
			//	attack_rec.x = attack_rec.x - (85 - attack_rec.width);
			//}
			//attack_rec.width = 85;
			//attack_rec.position += m_Direction * maths::vec2(15);
			//for (auto& monster : (*m_MonstersLayer))
			//{
			//	if (monster->check_collision(attack_rec))
			//	{
			//		std::cout << "attacking" << std::endl;
			//		monster->getHit(m_Direction, getDamage());
			//	}
			//}
		}

		void NetChar::setStuck(bool stuck_state)
		{
			if (stuck_state)
				m_Controls = { false };
			m_IsStuck = stuck_state;
		}

		void NetChar::pickItemUp()
		{
			//item::Item * item = m_ItemManager->getItem(m_Bounds.GetBottomMiddle());
			//if (item != nullptr)
			//	item->pickUp(&getBounds());
		}


		void NetChar::activateSkill(unsigned int skill_index)
		{
			if (m_Animation)
				return;
			if (m_SkillsTimeouts.find(skill_index) != m_SkillsTimeouts.end() ) // check for skill's timeout
				return; // Skill's criterions not met

			SRL::SkillInfo * skill_info = m_SkillManager->get_skill(skill_index);
			SRL::SkillPropertiesMap * item_properties = &skill_info->skill_properties;
			//if (item_properties->find(SRL::SkillDataType::mpCon) != item_properties->end())
			//	if (!m_CharacterStats->consumeMana(std::get<int>(item_properties->at(SRL::SkillDataType::mpCon))))
			//		return;

			if (item_properties->find(SRL::SkillDataType::timeOut) != item_properties->end())
				m_SkillsTimeouts[skill_index] = ATimer{ float(std::get<int>(item_properties->at(SRL::SkillDataType::timeOut)))};

			if (item_properties->find(SRL::SkillDataType::duration) != item_properties->end())
				m_SkillManager->add_icon(skill_info->name, skill_index, float(std::get<int>(item_properties->at(SRL::SkillDataType::duration))));

			SRL::AnimationMap* skill_animation_data = m_SkillManager->getAnimationData(skill_index);
			for (const auto& element : (*skill_animation_data))
			{
				switch (element.first)
				{
				case SRL::SkillAnimationTypes::effectAnimation:
					m_Animation.reset(new graphics::SpritedRenderable{ {-m_Bounds.width, 0 }, 
						skill_info->name + "_animation", element.second, true});
					break;
				case SRL::hitAnimation:
					if (skill_animation_data->find(SRL::ballAnimation) != skill_animation_data->end())
						activateAttackSkill(element.second, (*skill_animation_data)[SRL::ballAnimation], skill_info->name);
					break;
				default: ;
				}
			}
			//m_CharacterStats->activateSkill(skill_index, item_properties);
		}

		void NetChar::activateAttackSkill( SRL::FullAnimationData hit_animation_data, 
			SRL::FullAnimationData projectile_animation_data, const std::string& skill_name)
		{
			//std::vector<std::pair<float, Monster*>> monsters_in_range;
			//for (auto & monster : (*m_MonstersLayer))
			//{
			//	const maths::vec2 mon_pos = monster->getBounds().GetMiddle();
			//	const maths::vec2 char_pos = getBounds().GetMiddle();
			//	maths::vec2 dis_vec = char_pos - mon_pos;
			//	if (Right == m_Direction)
			//		dis_vec *= -1;
			//	if ((dis_vec.x < 500 && dis_vec.x > 0) && (dis_vec.y > -200 && dis_vec.y < 200))
			//	{
			//		float pyth = pow(dis_vec.x, 2) + pow(dis_vec.y, 2);
			//		monsters_in_range.emplace_back(std::make_pair(pyth, monster));
			//	}
			//}
			//if (!monsters_in_range.empty())
			//{
			//	Monster* hit_monster = std::min_element(monsters_in_range.begin(), monsters_in_range.end(),
			//		[](auto& pr1, auto& pr2) {return pr1.first < pr2.first; })->second;
			//	hit_monster->getHit(
			//		std::make_unique<skills::Projectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 },
			//			hit_monster, Damage{ 250, 90 }, m_Direction, skill_name, projectile_animation_data, hit_animation_data));
			//}
			//else
			//{
			//	m_Animations.add(
			//		std::make_unique<skills::Projectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 }, 
			//			m_Direction, skill_name, projectile_animation_data));
			//}
		}
	}
}
