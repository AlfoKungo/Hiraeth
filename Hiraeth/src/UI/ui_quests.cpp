#include "ui_quests.h"
#include "basic\EventManager.h"

namespace hiraeth {
	namespace ui {

		UiQuests::UiQuests(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 295, 396), control_key),
			m_Tab(SRL::Available)
		{
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd2.png")));

			unsigned int tab_x_values[] = {8, 84, 150, 206};
			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::QuestTab::Available; tab_index <= SRL::QuestTab::Recommended; ++tab_index)
			{
				SRL::QuestTab tab = static_cast<SRL::QuestTab>(tab_index);
				m_QuestTabs.insert(std::pair<SRL::QuestTab, std::unique_ptr<graphics::Group>>(tab, new graphics::Group()));
				m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(tab_x_values[tab_index], 350), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.Tab.disabled." + std::to_string(tab_index) + ".png")));
				m_QuestTabs[tab]->add(new graphics::Sprite(maths::vec2(tab_x_values[tab_index], 350), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.Tab.enabled." + std::to_string(tab_index) + ".png")));
			}
			fillGroup();
		}

		void UiQuests::fillGroup()
		{
			m_ForegroundGroup.clear();
			m_ForegroundGroup.add(m_QuestTabs[m_Tab].get());
		}

		void UiQuests::mouse_clicked(maths::vec2 mousePos)
		{
			for (auto const & tab : m_QuestTabs)
				if (tab.second.get()->m_Renderables[0]->getBounds().Contains(mousePos))
				{
					m_Tab = tab.first;
					fillGroup();
				}
		}
	}
}
