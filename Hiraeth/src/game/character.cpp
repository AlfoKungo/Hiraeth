#include "hrth_pch.h"
#include "character.h"
#include <utility>
#include "projectile.h"
#include "targeted_projectile.h"
#include "straight_projectile.h"
#include "character_graphics.h"

namespace hiraeth {
	namespace game {

		Character::Character(maths::vec2 pos, input::Keyboard* kb, map::MapLayer* map_layer,
			ui::UiEquip* ui_equip, ui::UiManager* ui_manager,
			item::ItemManager* item_manager, skills::SkillManager* skill_manager,
			CharacterStats* character_stats, std::map<unsigned int, Monster*>* monsters)
			: Creature(maths::Rectangle(pos, maths::vec2(32, 45)), map_layer, character_stats, true),
			m_Kb(kb),
			m_ItemManager(item_manager),
			m_UiEquip(ui_equip),
			m_UiManager(ui_manager),
			m_Monsters(monsters),
			m_SkillManager(skill_manager),
			m_CharacterStats(character_stats)
			//m_ClientHandler(client_handler)
			//m_Animations(m_TransformationMatrix)
		{
			FillCharacterTextureData(m_StatesRenderables);

			m_HitBox = maths::vec2{ 32, 45 };
			m_Org = maths::vec2{ m_HitBox.x / 2, 0 };
			m_HitSprite = graphics::Sprite{ maths::vec2(0, 0), getBounds().width,
				getBounds().height, 0x60ff4500 };

			for (auto [key, d] : *ui_equip->getEquips())
				Character::wearItem(key);
			
			registerKeys();
		}

		Character::~Character()
		{
		}

		void Character::update()
		{
			m_CharacterStats->update();
			if (m_Animation)
			{
				m_Animation->update();
				if (m_Animation->hasSpriteFinished())
					m_Animation.reset(nullptr);
			}
			m_Animations.update();
			m_Animations.clear_done();
			//m_Animations.m_Renderables.erase(
			//	std::remove_if(begin(m_Animations.m_Renderables), end(m_Animations.m_Renderables),
			//		[](const auto& sr) {return sr->hasSpriteFinished(); }), end(m_Animations.m_Renderables));

			//m_ProjectileAnimations.update();
			//for (auto iter = m_ProjectileAnimations.m_Renderables.begin(); iter != m_ProjectileAnimations.m_Renderables.end() ;)
			//{
			//	if ((*iter)->hasHitClashed())
			//	{
			//		//(*iter)->m_Enemy->addHit
			//		m_Animations.add(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0,0 },
			//			(*iter)->getSkillName() + "_hit", (*iter)->getHitAnimationData(), true));
			//		//Creature::getHit((*iter)->getDirection(), (*iter)->getDamage());
			//		(*iter)->m_Enemy->getHit((*iter)->getDirection(), (*iter)->getDamage());
			//		iter = m_ProjectileAnimations.m_Renderables.erase(iter);
			//	}
			//	else
			//		++iter;
			//}

			m_Speed = m_CharacterStats->getSpeed();
			m_Jump = m_CharacterStats->getJump();
			if (m_Controls.pick_up)
			{
				pickItemUp();
			}

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

			checkMobsCollision();

			Creature::update();
		}

		void Character::checkMobsCollision()
		{
			if (!is_hit)
				for (auto [key, monster] : *m_Monsters)
				{
					if (monster->check_collision(getHitBox()))
					{
						std::cout << "colliding" << std::endl;
						if (monster->getBounds().GetBottomMiddle().x < getBounds().GetBottomMiddle().x)
							getHit(Right, monster->getDamage());
						else
							getHit(Left, monster->getDamage());
						//m_ClientHandler->sendCharGotHit(m_CharacterStats->getStatsStruct_()->Hp);
						NetworkManager::Instance()->sendCharGotHit(m_CharacterStats->getStatsStruct_()->Hp);
						return;
					}
				}
		}

		void Character::draw(graphics::Renderer* renderer) const
		{
			Creature::draw(renderer);
			m_Animations.draw(renderer);
			//renderer->push(m_TransformationMatrix);
			if (m_Direction == Left)
				renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)));
			else
				renderer->push(m_TransformationMatrix * maths::mat4::Translate(maths::vec3(m_Org)) * 
					maths::mat4::Scale(maths::vec3(-1, 1, 1)) * maths::mat4::Translate(- maths::vec3(2 * m_Org)));
			if (m_Animation)
			{
				m_Animation->draw(renderer);
			}
			//for (auto [f, s] : m_Equips)
			//	s.draw(renderer);
			renderer->pop();
			m_ProjectileAnimations.draw(renderer);
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

		void Character::registerKeys()
		{
			//m_Kb->registerToKey(GLFW_KEY_UP, up, this);
			m_Kb->registerToKey(GLFW_KEY_DOWN, down, this);
			m_Kb->registerToKey(GLFW_KEY_LEFT, left, this);
			m_Kb->registerToKey(GLFW_KEY_RIGHT, right, this);
			m_Kb->registerToKey(GLFW_KEY_SPACE, jump, this);
			m_Kb->registerToKey(GLFW_KEY_LEFT_CONTROL, attack, this);
			m_Kb->registerToKey(GLFW_KEY_Z, pick_up, this);
			//std::vector<unsigned int> skill_keys{ GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_X, GLFW_KEY_B, GLFW_KEY_N,
			//	GLFW_KEY_T, GLFW_KEY_Y };
			//std::vector<unsigned int> available_skills = m_SkillManager->get_available_active_skills();
			//for (int i = 0; i < available_skills.size(); ++i)
			//{
			//	m_SkillKeysMap[i] = available_skills[i];
			//	m_SkillManager->m_UiSkills->transferSkillToKeyConfig(skill_keys[i],
			//		(controls_size)+i, available_skills[i], this);
			//}
		}
		void Character::loadSkillsToKeys()
		{
			std::vector<unsigned int> skill_keys{ GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_X, GLFW_KEY_B, GLFW_KEY_N,
				GLFW_KEY_T, GLFW_KEY_Y };
			std::vector<unsigned int> available_skills = m_SkillManager->get_available_active_skills();
			for (int i = 0; i < available_skills.size(); ++i)
			{
				m_SkillKeysMap[i] = available_skills[i];
				m_SkillManager->m_UiSkills->transferSkillToKeyConfig(skill_keys[i],
					(controls_size)+i, available_skills[i], this);
			}
		}

		void Character::carryOutAttack()
		{
			maths::Rectangle attack_rec = getBounds();
			network::Direction new_dir{ network::Direction::Right };
			if (Left == m_Direction)
			{
				new_dir = network::Direction::Left;
				attack_rec.x = attack_rec.x - (85 - attack_rec.width);
			}
			attack_rec.width = 85;
			attack_rec.position += m_Direction * maths::vec2(15);
			for (auto& [key, monster] : (*m_Monsters))
			{
				if (monster->check_collision(attack_rec))
				{
					std::cout << "attacking" << std::endl;
					const auto damage = getDamage();
					//monster->getHit(m_Direction, damage);
					attackMonsters(std::vector<Monster*>{monster}, std::vector<Damage>{damage});
					//m_ClientHandler->sendAttackPacket(network::MonsterDamage{float(damage.RawDamage), 
					NetworkManager::Instance()->sendAttackPacket(network::MonsterHit{ float(damage.RawDamage),
						monster->getId(), new_dir });
					//monster->getId(), 0, static_cast<network::Direction>(m_Direction) });
				// send the information to the server
				}
			}
		}

		void Character::setStuck(bool stuck_state)
		{
			if (stuck_state)
				m_Controls = { false };
			m_IsStuck = stuck_state;
		}

		//void Character::updateComplexSkills()
		//{
		//	std::vector<unsigned int> effect_skills = m_SkillManager->get_effect_skills();
		//	for (const auto& skill_id : effect_skills)
		//	{
		//	const auto skill_lvl = m_SkillManager->getSkillAlloc(skill_id);
		//		auto skill_info = m_SkillManager->get_skill_info(skill_id);
		//		for (const auto& prop : skill_info->skill_properties)
		//		{
		//			switch (prop.first)
		//			{
		//			case SRL::lifesteal:
		//				m_AttackModifiers[skill_id].push_back(std::make_pair(SRL::lifesteal,  getInt(prop.second, skill_lvl)));
		//				break;
		//			default:
		//				break;
		//			}
		//		}
		//	}
		//}

		void Character::attackMonster(Creature* attacked, Damage damage)
		{
			attacked->getHit(m_Direction, damage);
			auto attack_modifiers = m_CharacterStats->getAttackModifiers();
			for (const auto& [id, skill_mods] : attack_modifiers)
				for (const auto& mod : skill_mods)
				{
					switch (mod.first)
					{
					case SRL::lifesteal:
						m_Stats->recoverHp(damage.RawDamage * (float(mod.second) / 100.0f));
					default:
						break;
					}
				}
		}

		void Character::wearItem(SRL::EquipItemType item_type)
		{
			auto equip = m_UiEquip->getEquip(item_type);
			auto& equip_tex_data = equip->m_Textures[SRL::as_Default];
			//switch (item_type)
			//{
			//case SRL::Weapon:
			if (m_Equips.find(item_type) != m_Equips.end())
				m_Equips.erase(item_type);
			m_Equips.emplace(std::make_pair(item_type, graphics::Sprite{ {5, 40},equip_tex_data.org,
				graphics::TextureManager::Load(equip->getName() + "_def", equip_tex_data.tex_data) }));
			//	break;
			//case SRL::Hat:
			//	if (m_Equips.find(item_type) != m_Equips.end())
			//		m_Equips.erase(item_type);
			//	m_Equips.emplace(std::make_pair(SRL::Hat, graphics::Sprite{ {5, 40},equip_tex_data.org,
			//		graphics::TextureManager::Load(equip->getName() + "_def", equip_tex_data.tex_data)}));
			//	break;
			//default:
			//	break;
			//}
		}
		void Character::unWearItem(SRL::EquipItemType item_type)
		{
			if (m_Equips.find(item_type) != m_Equips.end())
				m_Equips.erase(item_type);
		}

		void Character::writeSay(const std::string say_msg)
		{
			Creature::writeSayBubble(say_msg);
		}

		void Character::attackMonsters(std::vector<Monster*> mobs, std::vector<Damage> damages)
		{
			auto attack_modifiers = m_CharacterStats->getAttackModifiers();
			for (int i = 0; i < mobs.size(); ++i)
			{
				mobs[i]->getHit(m_Direction, damages[i]);
				//for (const auto&[id, skill_mods] : m_AttackModifiers)
				for (const auto& [id, skill_mods] : attack_modifiers)
					for (const auto& mod : skill_mods)
						//for (const auto& mod : mods)
					{
						switch (mod.first)
						{
						case SRL::lifesteal:
							m_Stats->recoverHp(damages[i].RawDamage * (float(mod.second) / 100.0f));
						default:
							break;
						}
					}
			}
		}

		void Character::pickItemUp()
		{
			auto* item = m_ItemManager->getItem(m_Bounds.GetBottomMiddle());
			if (item != nullptr)
			{
				//auto item_loc = m_UiManager->getUiInventory()->findEmptyPosition(0);
				NetworkManager::Instance()->sendPickItem(item->getId());
				item->pickUp(&getBounds());
			}
		}

		int Character::getValueFromString(std::string str, unsigned int val)
		{
			auto pos = str.find('x');
			if (pos != std::string::npos)
				str.replace(pos, 1, std::to_string(val));
			return calculator::eval(str);
		}

		int Character::getInt(SRL::SkillPropertiesVar val, unsigned int skill_lvl)
		{

			if (std::holds_alternative<int>(val))
				return std::get<int>(val);
			std::string con_string = std::get<std::string>(val);
			return getValueFromString(con_string, skill_lvl);
		}


		void Character::activateSkill(unsigned int skill_id)
		{
			//if (!m_SkillActivationTimer.hasExpired() || // check for skill's activation time
			//if (!m_Animations.m_Renderables.empty())
			//	return;
			SRL::SkillInfo* skill_info = m_SkillManager->get_skill_info(skill_id);
			SRL::SkillPropertiesMap* skill_properties = &skill_info->skill_properties;
			const auto skill_lvl = m_SkillManager->getSkillAlloc(skill_id);

			if (skill_lvl == 0)
				return;
			if (m_StanceState == StcAttack)
				return;
			//if (m_Animation)
			//	return;
			if (m_SkillsTimeouts.find(skill_id) != m_SkillsTimeouts.end()) // check for skill's timeout
				return; // Skill's criterions not met
			if (skill_properties->find(SRL::SkillDataType::mpCon) != skill_properties->end())
			{
				if (!m_CharacterStats->consumeMana(getInt(skill_properties->at(SRL::SkillDataType::mpCon), skill_lvl)))
					return;
			}

			NetworkManager::Instance()->sendCharUseSkillE(skill_id, m_CharacterStats->getStatsStruct_()->Mp);

			if (skill_properties->find(SRL::SkillDataType::timeOut) != skill_properties->end())
				m_SkillsTimeouts[skill_id] = ATimer{ float(std::get<int>(skill_properties->at(SRL::SkillDataType::timeOut))) };
			//if (item_properties->find(SRL::SkillDataType::actTime) != item_properties->end())
			//	m_SkillActivationTimer = ATimer{ float(std::get<int>(item_properties->at(SRL::SkillDataType::actTime))) / 1000 };

			if (skill_properties->find(SRL::SkillDataType::duration) != skill_properties->end())
				m_SkillManager->add_icon(skill_info->name, skill_id, float(std::get<int>(skill_properties->at(SRL::SkillDataType::duration))));

			//for (const auto& element : (*skill_properties))
			//{
			//	switch (element.first)
			//	{
			//	case SRL::SkillDataType::timeOut:
			//		m_SkillsTimeouts[skill_index] = ATimer{ float(std::get<int>(skill_properties->at(SRL::SkillDataType::timeOut))) };
			//		break;
			//	case SRL::SkillDataType::duration:
			//		m_SkillManager->add_icon(skill_info->name, skill_index, float(std::get<int>(skill_properties->at(SRL::SkillDataType::duration))));
			//		break;
			//	default:
			//		break;
			//	}
			//}
			switch (skill_info->skill_type)
			{
			case SRL::active_attack_proj_targeted:
				break;
			case SRL::active_attack_swing:
				break;
			case SRL::active_buff:
				break;
			case SRL::active_move:
				activateTeleport(&skill_info->skill_properties, skill_lvl);
				break;
			default:
				break;
			}

			SRL::AnimationMap* skill_animation_data = m_SkillManager->getAnimationData(skill_id);
			for (const auto& element : (*skill_animation_data))
			{
				switch (element.first)
				{
				case SRL::SkillAnimationTypes::effectAnimation:
					m_Animation.reset(new graphics::SpritedRenderable{ {-m_Bounds.width, 0 },
						skill_info->name + "_animation", element.second, true });
					break;
				case SRL::hitAnimation:
					if (skill_animation_data->find(SRL::ballAnimation) != skill_animation_data->end())
					{
						change_stance_to_attack();
						auto monsters_hit = activateProjSkill(element.second, (*skill_animation_data)[SRL::ballAnimation],
							getInt(skill_properties->at(SRL::proj_range), skill_lvl), skill_info->name);
						NetworkManager::Instance()->sendCharUseSkillA(skill_id, monsters_hit);
					}
					else
					{
						change_stance_to_attack();
						auto monsters_hit = activateAttackSkill(element.second,
							200, skill_info->name);
						NetworkManager::Instance()->sendCharUseSkillA(skill_id, monsters_hit);
						for (auto mob : monsters_hit)
						{
							//m_Monsters[mob.monster_id].attack;
							attackMonster(m_Monsters->at(mob.monster_id), Damage{ unsigned int(mob.damage), 90});
						}
					}
					break;
				default:;
				}
			}
			m_CharacterStats->activateSkill(skill_id, skill_properties);
		}

		void Character::activateTeleport(SRL::SkillPropertiesMap* skill_properties, unsigned int skill_lvl)
		{
			auto m_x = getValueFromString(std::get<std::string>((*skill_properties)[SRL::move_x]), skill_lvl);
			auto m_y = getValueFromString(std::get<std::string>((*skill_properties)[SRL::move_y]), skill_lvl);
			float mv_y{ 0 };
			if (m_Kb->isKeyPressed(GLFW_KEY_UP))
			{
				mv_y = float(m_y) / 10;
				m_Force.y = 0.0f;
			}
			move(maths::vec2{ float(m_x) * m_Direction, mv_y });
		}

		std::vector<std::pair<float, Monster*>> Character::findMonstersInRange(maths::vec2 atk_range) const
		{
			std::vector<std::pair<float, Monster*>> monsters_in_range;
			for (auto& [key, monster] : (*m_Monsters))
			{
				const maths::vec2 mon_pos = monster->getBounds().GetMiddle();
				const maths::vec2 char_pos = getBounds().GetMiddle();
				maths::vec2 dis_vec = char_pos - mon_pos;
				if (Right == m_Direction)
					dis_vec *= -1;
				if ((dis_vec.x < atk_range.x && dis_vec.x > 0) && (dis_vec.y > -atk_range.y && dis_vec.y < atk_range.y))
				{
					float dis_pow = pow(dis_vec.x, 2) + pow(dis_vec.y, 2);
					monsters_in_range.emplace_back(std::make_pair(dis_pow, monster));
				}
			}
			return monsters_in_range;
		}

		std::vector<network::MonsterHit> Character::activateAttackSkill(SRL::FullAnimationData hit_animation_data,
			int range, const std::string& skill_name)
		{
			auto monsters_in_range = findMonstersInRange({ float(range), 200 });
			if (!monsters_in_range.empty())
			{
				Monster* hit_monster = std::min_element(monsters_in_range.begin(), monsters_in_range.end(),
					[](auto& pr1, auto& pr2) {return pr1.first < pr2.first; })->second;
				hit_monster->setStrikeAnimation(skill_name,
					std::move(hit_animation_data));
				return std::vector<network::MonsterHit>{network::MonsterHit{ 250.0f,
					hit_monster->getId(), static_cast<network::Direction>(m_Direction) }};
			}
			else
			{
				return {};
			}
		}

		//void Character::activateAttackSkill( SRL::FullAnimationData hit_animation_data, 
		std::vector<network::MonsterHit> Character::activateProjSkill(SRL::FullAnimationData hit_animation_data,
			const SRL::FullAnimationData& projectile_animation_data,
			int range, const std::string& skill_name)
		{
			auto monsters_in_range = findMonstersInRange({ float(range), 200 });
			if (!monsters_in_range.empty())
			{
				Monster* hit_monster = std::min_element(monsters_in_range.begin(), monsters_in_range.end(),
					[](auto& pr1, auto& pr2) {return pr1.first < pr2.first; })->second;
				//m_ProjectileAnimations.add(
					//std::make_unique<skills::TargetedProjectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 },
					//	m_Direction, skill_name, this, hit_monster, Damage{ 250, 90 }, projectile_animation_data, hit_animation_data));
				hit_monster->setProjectileAnimation(
					std::make_unique<skills::TargetedProjectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 },
						m_Direction, skill_name, this, hit_monster, Damage{ 250, 90 },
						projectile_animation_data, hit_animation_data, PREHIT_DELAY));
				return std::vector<network::MonsterHit>{network::MonsterHit{ 250.0f, hit_monster->getId(), static_cast<network::Direction>(m_Direction) }};
			}
			else
			{
				m_Animations.add(
					std::make_unique<skills::StraightProjectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 },
						m_Direction, skill_name, projectile_animation_data, 400, PREHIT_DELAY));
				return {};
			}
		}
	}
}
