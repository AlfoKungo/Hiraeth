﻿#pragma once
#include "ui_window.h"
#include <memory>
//#include "basic/EventManager.h"
#include <algorithm>
#include <iterator>
#include "srl/item_data.h"
//#include "item/item.h"
#include "ui_basic/ui_tabs.h"
#include "game/character_stats.h"
#include "ui_basic/ui_tab_inventory.h"


namespace hiraeth {
	namespace ui {
		class UiInventory : public UiWindow
		{
		private:
			UiTabs<UiTabInventory<item::ItemHold>>* m_Tabs;
			//UiTabs<UiTabInventory<item::Item>> m_Tabs;
			//UiTabs<item::Item> * m_Tabs;
			//std::map<unsigned int, unsigned int> m_ItemsOrd;
			std::pair<unsigned int, item::ItemHold*> m_HoldItem;
			game::CharacterStats* m_CharacterStats;
			//maths::vec2 m_OldItemPos;
		public:

			UiInventory(maths::vec2 pos, UiKey control_key, game::CharacterStats* character_stats);
			void fillGroup();
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override;
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void mouse_right_clicked(maths::vec2 mousePos) override;
			unsigned int addItem(item::ItemHold * new_item);
			//void addItem(item::Item * new_item, unsigned int pos_index);
			void addItem(unsigned int pos_index, item::ItemHold * new_item);
			//void addItem(item::Item * new_item, maths::vec2 new_item_pos);

			void draw(graphics::Renderer* renderer) const override 
			{ 
				UiWindow::draw(renderer);
			}
			//item::EquipItem* itemClickedOn(maths::vec2 mousePos);
			std::pair<unsigned int, item::EquipItem*> itemClickedOn(maths::vec2 mousePos);
			//item::EquipItem* getEquipItem(maths::vec2 mousePos);
			std::pair<unsigned int, item::EquipItem*> getEquipItem(maths::vec2 mousePos);
			unsigned int findEmptyPosition(unsigned int tab_type);
		private:
			//std::pair<unsigned int, std::unique_ptr<item::Item>> getItemPair(maths::vec2 mouse_pos);
			std::map<unsigned int, std::unique_ptr<item::ItemHold>>::iterator getItemPair(maths::vec2 mouse_pos);
			//item::Item * getItemByMousePos(maths::vec2 mousePos);
			void use_item(maths::vec2 mousePos);
			//void equip_item();
		};

	}
}
