#pragma once
#include "net_char.h"
#include "item/item_manager.h"

namespace hiraeth {
	namespace game {

		class NetCharManager
		{
		private:
			std::map<unsigned int, NetChar*> m_CharsMap;
			map::MapLayer* m_MapLayer;
			item::ItemManager *m_ItemManager;
			skills::SkillManager * m_SkillManager;
			graphics::Shader m_Shader;
			graphics::Layer<NetChar> m_Chars;
			std::map<unsigned int, Monster*>* m_Monsters;
		public:
			NetCharManager(map::MapLayer * map_layer, item::ItemManager *item_manager, 
				skills::SkillManager * skill_manager, std::map<unsigned int, Monster*>* monsters)
				: m_MapLayer{ map_layer },
				m_ItemManager(item_manager),
			m_SkillManager{skill_manager},
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_Chars(&m_Shader, true),
			m_Monsters(monsters)
			{
				
			}

			void update()
			{
				m_Chars.update();
			}

			void draw() const
			{
				m_Chars.render();
			}

			void addChar(unsigned int char_id, maths::vec2 char_pos)
			{
				//m_Chars.emplace(std::make_pair(char_id, {char_pos, m_MapLayer, m_SkillManager}));
				NetChar * temp = new NetChar{ char_pos, m_MapLayer, m_ItemManager, 
					m_SkillManager, m_Monsters };
				m_Chars.add(temp);
				m_CharsMap.insert(std::make_pair(char_id, temp));
			}

			void updateCharsState(unsigned int char_id, network::PlayerStateUpdateMsg char_state)
			{
				if (m_CharsMap.find(char_id) == m_CharsMap.end())
					addChar(char_id, char_state.pos);
				m_CharsMap[char_id]->setState(char_state);
			}

			void charUseSkillE(unsigned int char_id, unsigned int skill_id)
			{
				m_CharsMap[char_id]->activateSkill(skill_id);
			}
			void charUseSkillA(unsigned int char_id, network::AttackSkillMsg attack_msg)
			{
				m_CharsMap[char_id]->activateAttackSkill(attack_msg);
			}
			void charPickItem(unsigned int char_id, unsigned int item_id)
			{
				m_CharsMap[char_id]->pickItemUp(item_id);
			}
		};
	}
}
