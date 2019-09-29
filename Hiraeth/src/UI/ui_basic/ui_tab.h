#pragma once
#include "graphics/layers/group.h"
#include "graphics/layers/t_group.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace ui {

		typedef unsigned int TabIdType;

		template <class TabDataType = graphics::Renderable>
		class UiTab : public graphics::Renderable
		{
		public:
			graphics::Group m_Tab;
			graphics::Sprite * m_Header;
			graphics::TGroup<TabDataType> * m_TabContent;
			//std::map<unsigned int, TabDataType> m_ItemsOrd;

			UiTab(maths::vec2 tab_pos, std::string window_name, std::string tab_name, TabIdType tab_id)
				:
				m_Header(new graphics::Sprite(tab_pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + tab_name + ".Tab.enabled." + std::to_string(tab_id) + ".png"))),
				m_TabContent(new graphics::TGroup<TabDataType>())
			{
				m_Tab.add(m_Header);
				m_Tab.add(m_TabContent);
			}

			void draw(graphics::Renderer* renderer) const override
			{
				m_Tab.draw(renderer);
			}

			virtual void add_data(TabDataType* tab_data)
			{
				m_TabContent->add(tab_data);
			}
		};
	}
}