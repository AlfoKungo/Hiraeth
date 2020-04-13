#include "ui_quests.h"

#include "basic/EventManager.h"
#include "game/monsters/monster_data_manager.h"
#include "graphics/label.h"
#include "item/item_data_manager.h"

namespace hiraeth {
	namespace ui {

		UiQuests::UiQuests(maths::vec2 pos, UiKey control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 295, 396), control_key),
			m_DetailsGroup(new graphics::Group{}),
			//m_Tabs(new UiTabs<quest::QuestLabel>())
			m_DescriptionLabel {"arial", 16, "",{ 335, 320 },
				0xffffffff, graphics::Label::Alignment::LEFT },
			m_GoalsLabel {"arial", 14, "",{ 325, 250 },
				0xff000000, graphics::Label::Alignment::LEFT }
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0),
				graphics::TextureManager::Load(UI_LIST_BG1)));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5),
				graphics::TextureManager::Load(UI_LIST_BG2)));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(297, 0),
				graphics::TextureManager::Load(UI_INFO_BG1)));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(302, 5),
				graphics::TextureManager::Load(UI_INFO_BG2)));
			m_DetailsGroup->add(new graphics::Sprite(maths::vec2(306, 273),
				graphics::TextureManager::Load(UI_INFO_BG3)));
			m_DetailsGroup->add_ref(&m_DescriptionLabel);
			m_DetailsGroup->add_ref(&m_GoalsLabel);

			unsigned int tab_x_values[] = { 8, 84, 150, 206 };
			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::QuestTab::Available; tab_index <= SRL::QuestTab::Recommended; ++tab_index)
			{
				SRL::QuestTab tab = static_cast<SRL::QuestTab>(tab_index);
				m_Tabs.add_tab(tab_index, maths::vec2(tab_x_values[tab_index], 350), "Quests", "list", m_BackgroundGroup);
			}
			//for (int i = 0; i < 3; ++i)
			//{
			//	auto quest_data = SRL::deserial<SRL::QuestData>("quest", i);
			//	addAvailableQuest(i, quest_data);
			//}
			m_BackgroundGroup->add(&m_Tabs);
			m_BackgroundGroup->add(m_DetailsGroup);
			m_DetailsGroup->is_to_draw = false;

			EventManager* m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MonsterDied, this, &UiQuests::monsterDied);
		}

		void UiQuests::mouse_left_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y >348 && mousePos.y < 373)
				m_Tabs.mouse_left_clicked(mousePos);
			else
			{
				mousePos.y = (mousePos.y - 348) *(-1);
				unsigned int index = unsigned int(mousePos.y) / 17;
				for (const auto & quest_label: m_Tabs.getCurrentTabGroup()->m_TabContent->m_Renderables)
				{
					if (quest_label->get_location_index() == index)
					{
						m_DetailsGroup->is_to_draw = true;
						m_DescriptionLabel.setText(quest_label->getQuestData().description);
						m_GoalsLabel.setText("");
						for (auto prop : quest_label->getQuestData().quest_properties)
						{
							switch (prop.key)
							{
							case SRL::QuestProps::KillAmount: 
								addHuntGoalLabel(std::get<SRL::QuestDouble>(prop.value));
								break; 
							case SRL::QuestProps::GatherAmount:
								addGatherGoalLabel(std::get<SRL::QuestDouble>(prop.value));
								break;
							case SRL::QuestProps::TalkTo:
								break;
							case SRL::QuestProps::ExpReward:
								addExpRewardLabel(std::get<int>(prop.value));
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
				}
			}
		}

		void UiQuests::fillGroup()
		{
		}

		void UiQuests::addAvailableQuest(unsigned int quest_index, const SRL::QuestData& quest_data)
		{
			m_Quests.push_back(quest_data.name);
			//m_Tabs.getTabByIndex(0)->add_data(new quest::QuestLabel{ quest_index, static_cast<unsigned int>(m_Quests.size() - 1) });
			m_Tabs.getTabByIndex(0)->add_data(new quest::QuestLabel{ quest_index });
			//m_ForegroundGroup->add(new graphics::Label{ "arial", 19, quest_name, maths::vec2{ 14 , 332 - (float(m_Quests.size()) - 1)*17}, 0xff000000, graphics::Label::Alignment::LEFT });
		}

		void UiQuests::setQuestAsActive(unsigned int quest_id)
		{
			auto quest_data = SRL::deserial<SRL::QuestData>(DF_QUEST, quest_id);
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


		void UiQuests::setQuestsTab(const std::vector<unsigned>& quests_list, unsigned int quest_tab)
		{
			for (auto quest_id : quests_list)
			{
				auto quest_data = SRL::deserial<SRL::QuestData>("quest", quest_id);
				//m_Quests.push_back(quest_data.name);
				m_Tabs.getTabByIndex(quest_tab)->add_data(new quest::QuestLabel{ quest_id});
			}
		}

		void UiQuests::setQuestsInProgress(const std::vector<SRL::QuestDBStruct>& quests_list, unsigned int quest_tab)
		{
			for (auto [quest_id, quest_stage] : quests_list)
			{
				auto quest_data = SRL::deserial<SRL::QuestData>("quest", quest_id);
				//m_Quests.push_back(quest_data.name);
				m_Tabs.getTabByIndex(quest_tab)->add_data(new quest::QuestLabel{ quest_id});
			}
		}

		void UiQuests::addHuntGoalLabel(SRL::QuestDouble quest_double)
		{
			auto [mob, amount] = quest_double;
			auto mob_data = game::MonsterDataManager::Get(mob + 1);
			
			m_GoalsLabel.setText(m_GoalsLabel.getText() + "kill me please " + std::to_string(amount)
				+ " of " + mob_data.StatsStruct.Name + "\n");
		}

		void UiQuests::addGatherGoalLabel(SRL::QuestDouble quest_double)
		{
			auto [item_id, amount] = quest_double;
			auto item_data = item::ItemDataManager::Get(item_id + 1);
			
			m_GoalsLabel.setText(m_GoalsLabel.getText() + "gather me please " + std::to_string(amount)
				+ " of " + item_data.info.basic_item_info.item_name + "\n");
		}

		void UiQuests::addExpRewardLabel(int exp_reward)
		{
			m_GoalsLabel.setText(m_GoalsLabel.getText() + "I'll reward you " + std::to_string(exp_reward)
				+ " killos of dope" + "\n");
		}

		//void UiQuests::setQuestsAvailable(std::vector<unsigned> quests_avail)
		//{
		//	for (auto quest_id : quests_avail)
		//	{
		//		auto quest_data = SRL::deserial<SRL::QuestData>("quest", quest_id);
		//		m_Tabs.getTabByIndex(SRL::QuestTab::Available)->add_data(new quest::QuestLabel{ quest_id});
		//			//static_cast<unsigned int>(m_Quests.size() - 1) });
		//	}
		//}

		//void UiQuests::setQuestsInProgress(std::vector<unsigned int> quests_in_progress)
		//{
		//}

		//void UiQuests::setQuestsDone(std::vector<unsigned> quests_done)
		//{
		//}

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
