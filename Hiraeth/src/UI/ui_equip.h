#pragma once
#include "ui_window.h"
#include "game/character_stats.h"

#include <iostream>
#include "item/item.h"
#include "srl/equip_item_data.h"
#include "item/equip_item.h"

namespace hiraeth {
	namespace ui {

		class UiEquip : public UiWindow
		{
		private:
			//std::map<SRL::ItemType, item::Item*> m_Items;
			std::map<SRL::EquipItemType, item::EquipItem*> m_Items;
			std::map<SRL::EquipItemType, maths::vec2 > m_ItemsPositions;
			game::CharacterStats * m_CharacterStats{nullptr};
			graphics::TGroup<item::Item> m_Equips;
		public:
			UiEquip(maths::vec2 pos, UiKey control_key);
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			void fillGroup();
			void StatsUpdated();
			void addEquip(item::Item * item_to_equip)
			{
				auto equip_item = dynamic_cast<item::EquipItem*>(item_to_equip);
				item_to_equip->m_State = item::Item::InInventory;
				SRL::EquipItemType item_type = equip_item->getItemType();
				if (m_Items.find(item_type) != m_Items.end())
				{
					m_CharacterStats->unwearItem(m_Items[item_type]);
				}
				equip_item->setPosition(m_ItemsPositions[item_type]);
				m_CharacterStats->wearItem(equip_item);
				m_Items[item_type] = equip_item;
				m_Equips.add(equip_item);
			}
		};

	}
}
