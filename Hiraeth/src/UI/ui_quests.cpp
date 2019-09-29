#include "ui_quests.h"
#include "basic/EventManager.h"
#include "graphics/label.h"

namespace hiraeth {
	namespace ui {

		UiQuests::UiQuests(maths::vec2 pos, UiKey control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 295, 396), control_key)
			//m_Tabs(new UiTabs<quest::QuestLabel>())
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd2.png")));

			unsigned int tab_x_values[] = {8, 84, 150, 206};
			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::QuestTab::Available; tab_index <= SRL::QuestTab::Recommended; ++tab_index)
			{
				SRL::QuestTab tab = static_cast<SRL::QuestTab>(tab_index);
				m_Tabs.add_tab(tab_index, maths::vec2(tab_x_values[tab_index], 350), "Quests", "list", m_BackgroundGroup);
			}
			for (int i = 0; i < 3; ++i)
			{
				auto quest_data = SRL::deserial<SRL::QuestData>("quest", i);
				addAvailableQuest(i, quest_data);
			}
			m_BackgroundGroup->add(&m_Tabs);
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MonsterDied, this, &UiQuests::monsterDied);
		}

		void UiQuests::mouse_left_clicked(maths::vec2 mousePos)
		{
			m_Tabs.mouse_left_clicked(mousePos);
		}

		void UiQuests::fillGroup()
		{
		}

		void UiQuests::addAvailableQuest(unsigned int quest_index, const SRL::QuestData& quest_data)
		{
			m_Quests.push_back(quest_data.name);
			m_Tabs.getTabByIndex(0)->add_data(new quest::QuestLabel{ quest_index, static_cast<unsigned int>(m_Quests.size() - 1) });
			//m_ForegroundGroup->add(new graphics::Label{ "arial", 19, quest_name, maths::vec2{ 14 , 332 - (float(m_Quests.size()) - 1)*17}, 0xff000000, graphics::Label::Alignment::LEFT });
		}

		void UiQuests::setQuestAsActive(unsigned int quest_id)
		{
			auto quest_data = SRL::deserial<SRL::QuestData>("quest", quest_id);
			std::vector<std::unique_ptr<quest::QuestLabel>>* tab_rends = &m_Tabs.getTabByIndex(0)->m_TabContent->m_Renderables;
			auto result_quest = std::find_if(std::begin(*tab_rends),
				std::end(*tab_rends), [&](auto const& quest)
			{ return quest->get_index() == quest_id; });
			//m_Tabs->getTabByIndex(1)->add_data(new quest::QuestLabel{ quest_id, 1 });
			iterateOverQuestData(quest_id, (*result_quest)->getQuestData());
			m_Tabs.getTabByIndex(1)->m_TabContent->m_Renderables.insert(m_Tabs.getTabByIndex(1)->m_TabContent->m_Renderables.end(),
				std::make_move_iterator(result_quest), std::make_move_iterator(result_quest) + 1);// std::move(result_quest));
			tab_rends->erase(result_quest);
			relocateQuestLabels(0);
			//m_Tabs->getTabByIndex(1)->add_data(new quest::QuestLabel{quest_data, quest_index, m_Quests.size()});
		}

		void UiQuests::chargeGoal(unsigned int quest_id, SRL::QuestDouble quest_double)
		{
			//m_Goals.push_back(QuestGoal{ quest_double.type, quest_double.amount });
			m_QuestGoals[quest_id].push_back(QuestGoal{ quest_double.type, quest_double.amount });
		}

		void UiQuests::iterateOverQuestData(unsigned int quest_id, SRL::QuestData quest_data)
		{
			m_QuestGoals[quest_id] = std::vector<QuestGoal>{};
			for (SRL::QuestPropertiesMapPair element : (quest_data.quest_properties))
			{
				switch (element.key)
				{
				case SRL::QuestProps::KillAmount:
					chargeGoal(quest_id, std::get<SRL::QuestDouble>(element.value));
					break;
				case SRL::QuestProps::GatherAmount:
					break;
				case SRL::QuestProps::TalkTo:
					break;
				case SRL::QuestProps::ExpReward:
					break;
				case SRL::QuestProps::MoneyReward:
					break;
				case SRL::QuestProps::AchievementReward:
					break;
				default:
					break;
				}
			}
		}

		void UiQuests::relocateQuestLabels(unsigned int tab)
		{
			unsigned int index = 0;
			std::vector<std::unique_ptr<quest::QuestLabel>> * tab_rends = &m_Tabs.getTabByIndex(0)->m_TabContent->m_Renderables;
			for (auto & quest : *tab_rends)
				quest->relocateByIndex(index++);
			//std::ite
		}

		void UiQuests::monsterDied(unsigned int mob_id)
		{
			for (auto & quest_goals : m_QuestGoals)
			{
				for (auto & goal : quest_goals.second)
				{
					if (mob_id == goal.mob_id)
					{
						goal.mob_killed += 1;
					}
				}
			}
		}
	}
}
