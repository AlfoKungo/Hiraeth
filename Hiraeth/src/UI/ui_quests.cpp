#include "ui_quests.h"
#include "basic\EventManager.h"

namespace hiraeth {
	namespace ui {

		UiQuests::UiQuests(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 295, 396), control_key),
			m_Tabs(new UiTabs<graphics::Renderable>())
		{
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Quests/Quests.list.backgrnd2.png")));

			unsigned int tab_x_values[] = {8, 84, 150, 206};
			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::QuestTab::Available; tab_index <= SRL::QuestTab::Recommended; ++tab_index)
			{
				SRL::QuestTab tab = static_cast<SRL::QuestTab>(tab_index);
				m_Tabs->add_tab(tab_index, maths::vec2(tab_x_values[tab_index], 350), "Quests", "list", m_BackgroundGroup);
			}
			m_BackgroundGroup->add(m_Tabs);
		}

		void UiQuests::fillGroup()
		{
		}

		void UiQuests::mouse_clicked(maths::vec2 mousePos)
		{
			m_Tabs->mouse_clicked(mousePos);
		}
	}
}
