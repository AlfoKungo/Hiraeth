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
#include "basic/network_handler.h"
#include "basic/char_handler.h"


namespace hiraeth {
	namespace ui {
		class MainUi : public Updatable, public input::KeyboardEvent
		{
		private:
			input::Keyboard* m_Kb;
			game::CharacterStats m_CharacterStats;
			graphics::Sprite *m_Hp, *m_Mp, *m_Exp;
			graphics::Label *m_Name,* m_Job;
			graphics::Label* m_TypingLabel;
			game::StatsStruct* m_StatsStruct;
			graphics::Layer<graphics::Renderable> m_Layer;
			graphics::Group *m_GraphicGroup, *m_LabelGroup;
			bool m_IsTyping{false};
			std::string m_TextWritten;
			unsigned int m_TextCursor{ 0 };
			graphics::Sprite *m_CursorSprite;
		public:
			explicit MainUi(input::Keyboard* kb);
			void update() override;
			void draw() const;
			void StatsUpdated();
			void StatsUpdatedT(int X);
			game::CharacterStats * getCharacterStats() { return &m_CharacterStats; }
			void setCharacterStats(network::PlayerStats player_data, std::vector<unsigned int> stats_alloc)
			{
				m_StatsStruct->Name = player_data.name;
				m_Name->setText(player_data.name);
				m_StatsStruct->Level = player_data.char_lvl;
				m_Job->setText(getJobName(player_data.job));
				//m_StatsStruct->Job = player_data.job;
				m_StatsStruct->Job = getJobName(player_data.job);
				m_StatsStruct->Hp = player_data.hp;
				m_StatsStruct->Mp = player_data.mp;
				m_StatsStruct->Exp = player_data.exp;
				m_StatsStruct->Str = stats_alloc[0];
				m_StatsStruct->Dex = stats_alloc[1];
				m_StatsStruct->Int = stats_alloc[2];
				m_StatsStruct->Wit = stats_alloc[3];
				StatsUpdated();
				m_Kb->registerToKey(GLFW_KEY_ENTER, this);
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->execute(StatsUpdate);
			}

			virtual void ButtonClicked(input::Key control) override;

			virtual void ButtonReleased(input::Key control) override { }
		private:
			void setTypingState(bool new_state);

			void updateCursorPos();
			void fill_stats_group();
			std::string getJobName(unsigned int job_id);
			std::string create_stats_string(unsigned int value, unsigned int maxValue) const ;
		};
	}
}
