﻿#pragma once
#include "ui_window.h"
#include <memory>
//#include "basic/EventManager.h"
#include <algorithm>
#include <iterator>
#include "srl/item_data.h"
#include "item/item.h"
#include "ui_basic/ui_tabs.h"
#include "game/character_stats.h"


namespace hiraeth {
	namespace ui {
		class UiInventory : public UiWindow
		{
		private:
			UiTabs<item::Item> * m_Tabs;
			item::Item * m_HoldItem;
			game::CharacterStats *m_CharacterStats;
			maths::vec2 m_OldItemPos;
		public:

			UiInventory(maths::vec2 pos, UiKey control_key, game::CharacterStats *character_stats);
			void fillGroup();
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override;
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void mouse_right_clicked(maths::vec2 mousePos) override;
			void addItem(item::Item * new_item);

			void draw(graphics::Renderer* renderer) const override 
			{ 
				UiWindow::draw(renderer);
			}
		private:
			maths::vec2 findEmptyPosition(unsigned int tab_type) const;
			item::Item * getItemByMousePos(maths::vec2 mousePos);
			void use_item(maths::vec2 mousePos);
			void equip_item();
		};

	}
}