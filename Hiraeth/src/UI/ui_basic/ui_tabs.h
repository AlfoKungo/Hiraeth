#pragma once
#include "graphics/renderable.h"
#include "graphics/layers/group.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "ui_tab.h"

namespace hiraeth {
	namespace ui {


		//template <class TabDataType>
		template <class TabType = UiTab<graphics::Renderable>>
		class UiTabs : public graphics::Renderable
		{
		private:
			//std::map<TabIdType, UiTab<TabDataType>> m_Tabs;
			std::map<TabIdType, TabType> m_Tabs;
			TabIdType m_TabIndex;
			//UiTab<TabDataType> * m_ActiveTab;
			TabType * m_ActiveTab;
		public:
			UiTabs()
				: m_TabIndex(0),
				m_ActiveTab(nullptr)
			{

			}

			void draw(graphics::Renderer* renderer) const override 
			{ 
				m_ActiveTab->draw(renderer); 
			}

			void add_tab(TabIdType tab_id, maths::vec2 tab_pos, std::string window_name, std::string tab_name, graphics::Group * back_group)
			{
				m_Tabs.emplace(tab_id, TabType(tab_pos, window_name, tab_name, tab_id));
				//m_Tabs.emplace(tab_id, UiTab<TabDataType>(tab_pos, window_name, tab_name, tab_id));
				back_group->add(new graphics::Sprite(tab_pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + tab_name + ".Tab.disabled." + std::to_string(tab_id) + ".png")));
				updateActiveTab();
			}

			void mouse_left_clicked(maths::vec2 mousePos)
			{

				for (auto const & tab : m_Tabs)
					if (tab.second.m_Header->getBounds().Contains(mousePos))
					{
						m_TabIndex = tab.first;
						updateActiveTab();
					}
			}

			TabType * getTabByIndex(TabIdType tab_index) 
			//UiTab<TabDataType> * getTabByIndex(TabIdType tab_index) 
			{
			return &m_Tabs.find(tab_index)->second;
			}

			TabType * getCurrentTabGroup()
			//UiTab<TabDataType> * getCurrentTabGroup()
			{
				return getTabByIndex(m_TabIndex);
			}

			TabIdType getTabIndex() const
			{
				return m_TabIndex;
			}
		private:
			void updateActiveTab()
			{
				m_ActiveTab = &m_Tabs.find(m_TabIndex)->second;
			}
		};
	}
}