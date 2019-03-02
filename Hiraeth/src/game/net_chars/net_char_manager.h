#pragma once
#include "net_char.h"

namespace hiraeth {
	namespace game {

		class NetCharManager
		{
		private:
			std::map<unsigned int, NetChar*> m_CharsMap;
			map::MapLayer* m_MapLayer;
			skills::SkillManager * m_SkillManager;
			graphics::Shader m_Shader;
			graphics::Layer<NetChar> m_Chars;
		public:
			NetCharManager(map::MapLayer * map_layer, skills::SkillManager * skill_manager)
				: m_MapLayer{ map_layer },
			m_SkillManager{skill_manager},
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_Chars(&m_Shader, true)
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
				NetChar * temp = new NetChar{ char_pos, m_MapLayer, m_SkillManager };
				m_Chars.add(temp);
				m_CharsMap.insert(std::make_pair(char_id, temp));
			}

			void updateCharsState(unsigned int char_id, network::PlayerStateUpdateMsg char_state)
			{
				if (m_CharsMap.find(char_id) == m_CharsMap.end())
					addChar(char_id, char_state.pos);
				m_CharsMap[char_id]->setState(char_state);
			}
		};
	}
}
