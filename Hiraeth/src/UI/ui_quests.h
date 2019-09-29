#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "srl/quest_data.h"
#include "ui_basic/ui_tabs.h"
#include "quest/quest_label.h"

namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 3
		struct QuestGoal
		{
			unsigned int mob_id;
			unsigned int mob_amount;
			unsigned int mob_killed{ 0 };
		};
		struct QuestReward
		{
			unsigned int ExpReward{0};
			unsigned int MoneyReward{0};
			std::vector<unsigned int> ItemRewards;
		};

		class UiQuests : public UiWindow
		{
		private:
			//UiTabs<quest::QuestLabel> * m_Tabs;
			//UiTabs<quest::QuestLabel>  m_Tabs;
			UiTabs<UiTab<quest::QuestLabel>> m_Tabs;
			std::vector<std::string> m_Quests;
			//std::vector<QuestGoal> m_Goals;
			std::map<unsigned int, std::vector<QuestGoal>> m_QuestGoals;
		public:
			UiQuests(maths::vec2 pos, UiKey control_key);
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			void fillGroup();
			void addAvailableQuest(unsigned int quest_index, const SRL::QuestData& quest_data);
			void setQuestAsActive(unsigned int quest_id);
			void chargeGoal(unsigned int quest_id, SRL::QuestDouble quest_double);
			void iterateOverQuestData(unsigned int quest_id, SRL::QuestData quest_data);
			void relocateQuestLabels(unsigned int tab);
			void monsterDied(unsigned int mob_id);
		};

	}
}
