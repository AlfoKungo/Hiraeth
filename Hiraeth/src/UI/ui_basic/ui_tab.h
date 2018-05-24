#pragma once
#include "graphics/layers/group.h"
#include "graphics/layers/t_group.h"

namespace hiraeth {
	namespace ui {

		template <class TabData>
		class UiTab
		{
			graphics::Group m_Tab;
			graphics::TGroup<TabData> m_TabData;

			UiTab()
			{
				m_Tab.add(std::pair<unsigned int, std::unique_ptr<graphics::Group>>(tab_id, new graphics::Group()));
				m_Tab.add(new graphics::Sprite(tab_pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + tab_name + ".Tab.enabled." + std::to_string(tab_id) + ".png")));
				updateActiveTab();
			}
		};
	}
}