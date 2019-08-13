#pragma once
#include "basic/drawable.h"
#include "basic/updatable.h"
#include "graphics/renderable.h"
#include <vector>
#include "graphics/layers/layer.h"
#include "graphics/layers/group.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "game/character_stats.h"
#include "graphics/label.h"
#include <string>
#include "net/protocol.h"


namespace hiraeth {
	namespace ui {
		class MainUi : public Updatable
		{
		private:
			game::CharacterStats m_CharacterStats;
			graphics::Sprite *m_Hp, *m_Mp, *m_Exp;
			graphics::Label *m_Name,* m_Job;
			game::StatsStruct* m_StatsStruct;
			graphics::Layer<graphics::Renderable> m_Layer;
			graphics::Group *m_GraphicGroup, *m_LabelGroup;
		public:
			explicit MainUi();
			void update() override;
			void draw() const;
			void StatsUpdated();
			void StatsUpdatedT(int X);
			game::CharacterStats * getCharacterStats() { return &m_CharacterStats; }
			void setCharacterStats(network::PlayerData player_data)
			{
				m_StatsStruct->Name = player_data.name;
				m_Name->setText(player_data.name);
				m_StatsStruct->Level = player_data.char_lvl;
				m_Job->setText(player_data.job);
				m_StatsStruct->Job = player_data.job;
				m_StatsStruct->Hp = player_data.hp;
				m_StatsStruct->Mp = player_data.mp;
				m_StatsStruct->Exp = player_data.exp;
				StatsUpdated();
			}
		private:
			void fill_stats_group();
			std::string create_stats_string(unsigned int value, unsigned int maxValue) const ;
		};
	}
}
