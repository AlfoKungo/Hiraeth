#pragma once
#include "ui_window.h"
#include <memory>
//#include "basic/EventManager.h"
#include <algorithm>
#include <iterator>
#include "srl/item_data.h"
#include "ui_basic/ui_tabs.h"
#include "game/character_stats.h"
#include "ui_basic/ui_tab_inventory.h"


namespace hiraeth {
	namespace ui {
		//const float WIDTH{ 172.0f }, HEIGHT{ 335.0f };
		const float UI_INVENTORY_WIDTH{ 172.0f }, UI_INVENTORY_HEIGHT{ 335.0f };
		class UiInventory : public UiWindow
		{
		private:
			UiTabs<UiTabInventory>* m_Tabs;
			//UiTabs<UiTabInventory<item::Item>> m_Tabs;
			//UiTabs<item::Item> * m_Tabs;
			//std::map<unsigned int, unsigned int> m_ItemsOrd;
			std::pair<unsigned int, item::ItemHold*> m_HoldItem;
			game::CharacterStats* m_CharacterStats;
			maths::Rectangle* m_UiTradeRec;
			maths::Rectangle* m_UiEquipRec;

			maths::vec2 * m_UiTradePos;
			bool * m_IsTradeAble;
			//maths::vec2 m_OldItemPos;
		public:

			UiInventory(maths::vec2 pos, UiKey control_key, game::CharacterStats* character_stats);
			void fillGroup();
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override;
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void mouse_right_clicked(maths::vec2 mousePos) override;
			bool isWindowContains(maths::vec2 pos) const override;
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
			//unsigned int findEmptyPosition(unsigned int tab_type);

			void setTradeUi(maths::vec2* pos, bool* is_trading)
			{
				m_UiTradePos = pos;
				m_IsTradeAble = is_trading;
			}
		private:
			//std::pair<unsigned int, std::unique_ptr<item::Item>> getItemPair(maths::vec2 mouse_pos);
			std::map<unsigned int, std::unique_ptr<item::ItemHold>>::iterator getItemPair(maths::vec2 mouse_pos);
			//item::Item * getItemByMousePos(maths::vec2 mousePos);
			void use_item(maths::vec2 mousePos);
			//void equip_item();
		};

	}
}
