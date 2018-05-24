#pragma once
#include "graphics/renderable.h"
#include "graphics/layers/group.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth {
	namespace ui {

		typedef unsigned int TabType;

		//template <class TabType>
		class UiTabs : public graphics::Renderable
		{
		private:
			std::map<TabType, std::unique_ptr<graphics::Group>> m_Tabs;
			TabType m_TabIndex;
			graphics::Group * m_ForegroundGroup;
		public:
			UiTabs(graphics::Group * foreground_group)
				: m_ForegroundGroup(foreground_group),
				m_TabIndex(0)
			{

			}

			void add_tab(unsigned int tab_id, maths::vec2 tab_pos, std::string window_name, std::string tab_name, graphics::Group * back_group)
			{
				m_Tabs.insert(std::pair<unsigned int, std::unique_ptr<graphics::Group>>(tab_id, new graphics::Group()));
				back_group->add(new graphics::Sprite(tab_pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + tab_name + ".Tab.disabled." + std::to_string(tab_id) + ".png")));
				m_Tabs[tab_id]->add(new graphics::Sprite(tab_pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + tab_name + ".Tab.enabled." + std::to_string(tab_id) + ".png")));
				updateActiveTab();
			}

			void mouse_clicked(maths::vec2 mousePos)
			{

				for (auto const & tab : m_Tabs)
					if (tab.second.get()->m_Renderables[0]->getBounds().Contains(mousePos))
					{
						m_TabIndex = tab.first;
						updateActiveTab();
					}
			}

			graphics::Group * getCurrentTabGroup()
			{
				return m_Tabs[m_TabIndex].get();
			}

			graphics::Group * getTabByIndex(TabType tab_index)
			{
				return m_Tabs[tab_index].get();
			}
		private:
			void updateActiveTab()
			{
				m_ForegroundGroup->clear();
				m_ForegroundGroup->add(m_Tabs[m_TabIndex].get());
			}
		};
	}
}