#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "srl/quest_data.h"
#include "ui_basic/ui_tabs.h"
#include "quest/quest_label.h"
#include "ui_basic/ui_tab_quests.h"

namespace hiraeth {
	namespace ui {

#define TAB_BEG 0
#define TAB_END 3
		constexpr auto UI_LIST_BG1 = "Assets/UI/Quests/Quests.list.backgrnd.png";
		constexpr auto UI_LIST_BG2 = "Assets/UI/Quests/Quests.list.backgrnd2.png";
		constexpr auto UI_INFO_BG1 = "Assets/UI/Quests/Quests.quest_info.backgrnd.png";
		constexpr auto UI_INFO_BG2 = "Assets/UI/Quests/Quests.quest_info.backgrnd2.png";
		constexpr auto UI_INFO_BG3 = "Assets/UI/Quests/Quests.quest_info.backgrnd3.png";
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
			UiTabs<UiTabQuests> m_Tabs;
			std::vector<std::string> m_Quests;
			graphics::Group* m_DetailsGroup;
			graphics::Label m_DescriptionLabel;
			graphics::Label m_GoalsLabel;
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
			void setQuestsTab(const std::vector<unsigned>& quests_list, unsigned quest_tab);
			void setQuestsInProgress(const std::vector<SRL::QuestDBStruct>& quests_list, unsigned quest_tab);
			void setQuestAsDone(unsigned int quest_id);
			//void setQuestsAvailable(std::vector<unsigned int> quests_avail);
			//void setQuestsInProgress(std::vector<unsigned int> quests_in_progress);
			//void setQuestsDone(std::vector<unsigned int> quests_done);
			void chargeGoal(unsigned int quest_id, SRL::QuestDouble quest_double);
			void iterateOverQuestData(unsigned int quest_id, SRL::QuestData quest_data);
			void relocateQuestLabels(unsigned int tab);
			void monsterDied(unsigned int mob_id);
		private:
			void addHuntGoalLabel(SRL::QuestDouble quest_double);
			void addGatherGoalLabel(SRL::QuestDouble quest_double);
			void addExpRewardLabel(int exp_reward);
		};
	}
}
