#pragma once
#include "UI/ui_quests.h"

namespace hiraeth {
	namespace item {

		class QuestManager
		{
		private:
			ui::UiQuests* m_UiQuest;
			std::vector<unsigned int> quests_in_progress;
			//std::vector<unsigned int> quests_done;
		public:
			QuestManager(ui::UiQuests* ui_quest)
				: m_UiQuest(ui_quest)
			{
				
			}
		};
	}
}
