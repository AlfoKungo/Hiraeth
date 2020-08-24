#pragma once
#include "net_char.h"
#include "item/item_manager.h"
#include "basic/network_handler.h"

namespace hiraeth {
	namespace game {

		class NetCharManager : public input::MouseEvent
		{
		private:
			std::map<unsigned int, NetChar*> m_CharsMap;
			map::MapLayer* m_MapLayer;
			//item::ItemManager *m_ItemManager;
			skills::SkillManager * m_SkillManager;
			graphics::Shader m_Shader;
			graphics::Layer<NetChar> m_Chars;
			std::map<unsigned int, Monster*>* m_Monsters;
		public:
			NetCharManager(map::MapLayer * map_layer, input::Keyboard* kb, 
				skills::SkillManager * skill_manager, std::map<unsigned int, Monster*>* monsters)
				: m_MapLayer{ map_layer },
				//m_ItemManager(item_manager),
			m_SkillManager{skill_manager},
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_Chars(&m_Shader, true),
			m_Monsters(monsters)
			{
				kb->registerToMouse(this);
			}

			void update()
			{
				m_Chars.update();
			}

			void draw() const
			{
				m_Chars.render();
			}

			virtual bool leftButtonClicked(float mx, float my) override{return false;}
			virtual bool rightButtonClicked(float mx, float my) override
			{
				const maths::vec2 translated_pos = view::Camera::mouse_to_map_position(maths::vec2(mx, my));
				for (const auto& [char_id, char_data] : m_CharsMap)
					if (char_data->getBounds().Contains(translated_pos))
					{
						NetworkManager::Instance()->sendRequestParty(char_id);
					}
				return false;
			}
			virtual bool leftButtonReleased(float mx, float my) override {return false;}
			virtual bool mouseMove(float pmx, float pmy, float mx, float my) override {return false;}
			virtual bool is_window_contains(maths::vec2 mouse_pos) const override {return false;}

			void addChar(unsigned int char_id, maths::vec2 char_pos)
			{
				//m_Chars.emplace(std::make_pair(char_id, {char_pos, m_MapLayer, m_SkillManager}));
				NetChar * temp = new NetChar{ char_pos, m_MapLayer, 
					m_SkillManager, m_Monsters };
				m_Chars.add(temp);
				m_CharsMap.insert(std::make_pair(char_id, temp));
			}

			void clearChars()
			{
				m_Chars.clear();
				m_CharsMap.clear();
			}

			void deleteChar(unsigned int player_id)
			{
				m_Chars.m_Renderables.erase(std::remove(m_Chars.m_Renderables.begin(), 
					m_Chars.m_Renderables.end(), m_CharsMap[player_id]), m_Chars.m_Renderables.end());
				m_CharsMap.erase(player_id);
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
				//m_CharsMap[char_id]->pickItemUp(item_id);
				//item::Item * item = m_ItemManager->getItem(item_id);
				//item->pickUp(&m_CharsMap[char_id]->getBounds());
			}
			std::map<unsigned int, NetChar*>& getCharsMap() { return m_CharsMap; }
		};
	}
}
