#pragma once
#include "ui_window.h"
#include <memory>
//#include "basic/EventManager.h"
#include <algorithm>
#include <iterator>
#include "srl/item_data.h"
#include "item/item.h"
#include "ui_basic/ui_tabs.h"


namespace hiraeth {
	namespace ui {
		class UiInventory : public UiWindow
		{
		private:
			//std::map<SRL::ItemTab, std::unique_ptr<graphics::Group>> m_ItemTabs;
			//SRL::ItemTab m_Tab;
			UiTabs m_Tabs;
			item::Item * m_HoldItem;
			maths::vec2 m_OldItemPos;
		public:
			UiInventory(maths::vec2 pos, input::Controls control_key);
			void fillGroup();
			void mouse_clicked(maths::vec2 mousePos) override;
			void mouse_released(maths::vec2 mousePos) override;
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void addItem(item::Item * new_item);
		};

	}
}