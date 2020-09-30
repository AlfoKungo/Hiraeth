#include "hrth_pch.h"
#include "net_char.h"
#include "game/character_graphics.h"

namespace hiraeth {
	namespace game {

		NetChar::NetChar(maths::vec2 pos, map::MapLayer *map_layer,
			skills::SkillManager *skill_manager, std::map<unsigned int, Monster*>* monsters)
			: Creature(maths::Rectangle(pos, maths::vec2(32, 45)), map_layer, new CharacterStats{}, true),
			//m_ItemManager(item_manager),
			m_SkillManager(skill_manager), m_Monsters(monsters)
			//m_Animations(m_TransformationMatrix)
		{
			FillCharacterTextureData(m_StatesRenderables);

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

		void NetChar::pickItemUp(unsigned int item_id)
		{
			//item::Item * item = m_ItemManager->getItem(item_id);
			//if (item != nullptr)
			//	item->pickUp(&getBounds());
		}


		void NetChar::activateSkill(unsigned int skill_index)
		{
			if (m_Animation)
				return;
			if (m_SkillsTimeouts.find(skill_index) != m_SkillsTimeouts.end() ) // check for skill's timeout
				return; // Skill's criterions not met

			SRL::SkillInfo * skill_info = m_SkillManager->get_skill_info(skill_index);
			//SRL::SkillPropertiesMap * item_properties = &skill_info->skill_properties;
			//if (item_properties->find(SRL::SkillDataType::mpCon) != item_properties->end())
			//	if (!m_CharacterStats->consumeMana(std::get<int>(item_properties->at(SRL::SkillDataType::mpCon))))
			//		return;

			//if (item_properties->find(SRL::SkillDataType::timeOut) != item_properties->end())
			//	m_SkillsTimeouts[skill_index] = ATimer{ float(std::get<int>(item_properties->at(SRL::SkillDataType::timeOut)))};

			//if (item_properties->find(SRL::SkillDataType::duration) != item_properties->end())
			//	m_SkillManager->add_icon(skill_info->name, skill_index, float(std::get<int>(item_properties->at(SRL::SkillDataType::duration))));

			SRL::AnimationMap* skill_animation_data = m_SkillManager->getAnimationData(skill_index);
			for (const auto& element : (*skill_animation_data))
			{
				switch (element.first)
				{
				case SRL::SkillAnimationTypes::effectAnimation:
					m_Animation.reset(new graphics::SpritedRenderable{ {-m_Bounds.width, 0 }, 
						skill_info->name + "_animation", element.second, true});
					break;
				//case SRL::hitAnimation:
				//	if (skill_animation_data->find(SRL::ballAnimation) != skill_animation_data->end())
				//		activateAttackSkill(element.second, (*skill_animation_data)[SRL::ballAnimation], skill_info->name);
				//	break;
				default: ;
				}
			}
			//m_CharacterStats->activateSkill(skill_index, item_properties);
		}

		void NetChar::activateAttackSkill( network::AttackSkillMsg attack_msg)
		{
			auto animation_data = m_SkillManager->getAnimationData(attack_msg.skill_id);
			auto hit_animation_data = (*animation_data) [SRL::hitAnimation];
			auto projectile_animation_data = (*animation_data) [SRL::ballAnimation];
			auto skill_name = m_SkillManager->get_skill_info(attack_msg.skill_id)->name;
			if (attack_msg.monsters_hit.empty())
			{
				m_Animations.add(
					std::make_unique<skills::BaseProjectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 }, 
						m_Direction, skill_name, projectile_animation_data, PREHIT_DELAY));
			}
			else
			{
				for (const auto& monster_hit : attack_msg.monsters_hit)
				{
					Monster* hit_monster = (*m_Monsters)[monster_hit.monster_id];
					hit_monster->setProjectileAnimation(
						std::make_unique<skills::TargetedProjectile>(getBounds().GetMiddle() - maths::vec2{ 20, 0 },
							static_cast<Direction>(monster_hit.dir), skill_name, this, hit_monster, 
							Damage{unsigned int( monster_hit.damage), 90 }, projectile_animation_data, hit_animation_data, PREHIT_DELAY));
					hit_monster->getHit(static_cast<game::Direction>(monster_hit.dir), { unsigned int(monster_hit.damage), 100 });
				}
				
			}
		}
	}
}
