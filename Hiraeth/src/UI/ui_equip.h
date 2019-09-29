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
			std::map<SRL::EquipItemType, item::EquipItem*> m_Equips;
			std::map<SRL::EquipItemType, maths::vec2 > m_ItemsPositions;
			game::CharacterStats * m_CharacterStats{nullptr};
			//graphics::TGroup<item::Item> m_Equips;
		public:
			UiEquip(maths::vec2 pos, UiKey control_key, game::CharacterStats *character_stats);
			void mouse_left_clicked(maths::vec2 mousePos) override;
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override;
			void fillGroup();
			void StatsUpdated();
			item::EquipItem* getEquip(SRL::EquipItemType equip_type)
			{
				return m_Equips[equip_type];
			}
			item::EquipItem* addEquip(item::EquipItem * equip_item)
			{
				//equip_item->m_State = item::Item::InInventory;
				const SRL::EquipItemType item_type = equip_item->getItemType();
				item::EquipItem * old_item = nullptr;
				if (m_Equips.find(item_type) != m_Equips.end())
				{
					old_item = m_Equips[item_type];
					m_CharacterStats->unwearItem(m_Equips[item_type]);
				}
				equip_item->setPosition(m_ItemsPositions[item_type]);
				m_CharacterStats->wearItem(equip_item);
				m_Equips[item_type] = equip_item;
				//m_Equips.add(equip_item);
				fillGroup();
				return old_item;
			}
			item::EquipItem* unEquip(maths::vec2 mouse_pos)
			{
				auto result_item = std::find_if(std::begin(m_ItemsPositions),
					std::end(m_ItemsPositions), [&](auto const& element)
				{ return maths::Rectangle{ element.second, maths::vec2{32} }.Contains(mouse_pos); });
				item::EquipItem* return_item{ nullptr };
				if (result_item != std::end(m_ItemsPositions))
				{
					return_item = m_Equips[(result_item)->first];
					return_item->setDrawDetails(false);
					m_CharacterStats->unwearItem(return_item);
					m_Equips.erase((*result_item).first);
					fillGroup();
				}
				return return_item;
			}
			std::map<SRL::EquipItemType, item::EquipItem*>& getEquips(){ return m_Equips; }
		};

	}
}
