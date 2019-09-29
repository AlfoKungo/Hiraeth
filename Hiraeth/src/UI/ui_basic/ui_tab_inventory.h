#pragma once
#include "ui_tab.h"
#include "graphics/layers/mt_group.h"

namespace hiraeth {
	namespace ui {

		template <class TabDataType = graphics::Renderable>
		class UiTabInventory : public UiTab<TabDataType>
		{
		public:
			//std::map<unsigned int, TabDataType*> m_ItemsOrd;
			graphics::MTGroup<TabDataType> *m_MtGroup;
			UiTabInventory(maths::vec2 tab_pos, const std::string& window_name, const std::string& tab_name, TabIdType tab_id)
				: UiTab(tab_pos, window_name, tab_name, tab_id),
				m_MtGroup(new graphics::MTGroup<TabDataType>)
			{	
				m_Tab.add(m_MtGroup);
			}

			void add_data(TabDataType* tab_data) override
			{
				//UiTab<TabDataType>::add_data(tab_data);
				//m_ItemsOrd[loc] = tab_data;
				auto loc = findAvailableLoc();
				tab_data->setPosition(getPosByLocIndex(loc));
				m_MtGroup->add(loc, tab_data);
			}
			unsigned int add_data_b(TabDataType* tab_data) 
			{
				auto loc = findAvailableLoc();
				tab_data->setPosition(getPosByLocIndex(loc));
				m_MtGroup->add(loc, tab_data);
				return loc;
			}
			void add_data(unsigned int loc, TabDataType* tab_data)
			{
				tab_data->setPosition(getPosByLocIndex(loc));
				m_MtGroup->add(loc, tab_data);
			}

			unsigned int findAvailableLoc() const
			{
				unsigned int val = 0;
				for (const auto& [key, d] : m_MtGroup->m_Renderables)
				{
					if (key != val)
						break;
					val++;
				}
				return val;
			}

			static maths::vec2 getPosByLocIndex(unsigned int loc_index)
			{
				return maths::vec2{ float(loc_index % 4) * 36 + 11, 250.0f - float(loc_index / 4) * 35 };
			}

			std::pair<unsigned int, maths::vec2> findEmptyPosition()
			{
				auto loc_index = findAvailableLoc();
				maths::vec2 pos(float(loc_index % 4) * 36 + 11, 250.0f - float(loc_index / 4) * 35);
				return std::make_pair(loc_index, pos);
			}
			void changeItemPos(unsigned int index1, unsigned int index2)
			{
				auto nodeHandler = m_MtGroup->m_Renderables.extract(index1);
				nodeHandler.key() = index2;
				m_MtGroup->m_Renderables.insert(std::move(nodeHandler));
				m_MtGroup->m_Renderables[index2]->setPosition(getPosByLocIndex(index2));
			}
			void switchItems(unsigned int index1, unsigned int index2)
			{
				
			}
			unsigned int getPosIndexByMousePos(maths::vec2 mouse_pos)
			{
				unsigned int row = (mouse_pos.x - 11) / 36;
				unsigned int line = (283 - mouse_pos.y ) / 35;
				if (row >= 4 || mouse_pos.y > 283)
					return 1000;
				return row + line * 4;
			}
		};
	}
}
