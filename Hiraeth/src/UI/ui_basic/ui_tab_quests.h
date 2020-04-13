#pragma once
#include "ui_tab.h"

namespace hiraeth {
	namespace ui {
		
		using namespace quest;
		//template <class TabDataType = graphics::Renderable>
		//class UiTabQuests : public UiTab<TabDataType>
		class UiTabQuests : public UiTab<QuestLabel>
		{
		public:
			//graphics::MTGroup<QuestLabel> m_MtGroup;
			UiTabQuests(maths::vec2 tab_pos, const std::string& window_name, const std::string& tab_name, TabIdType tab_id)
				: UiTab<QuestLabel>(tab_pos, window_name, tab_name, tab_id)
			{
				
			}

			void add_data(QuestLabel* tab_data) override
			{
				const auto place_index = m_TabContent->m_Renderables.size();
				UiTab<QuestLabel>::add_data(tab_data);
				tab_data->setPosition(maths::vec2{ 14 , 315 - (float(place_index) - 1) * 17 });
			}
		};
	}
}
