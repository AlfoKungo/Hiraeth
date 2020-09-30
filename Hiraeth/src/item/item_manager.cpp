#include "hrth_pch.h"
#include "item_manager.h"

namespace hiraeth {
	namespace item {

		ItemManager::ItemManager(const std::vector<physics::FootHold>& foot_holds, 
			ui::UiInventory * inventory, ui::UiEquip * equip)
			:
			m_DroppedItems(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
			m_FootHolds(foot_holds),
			m_Inventory(inventory),
			m_Equip(equip)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &ItemManager::mapChanged);

		}

		void ItemManager::draw() const
		{
			m_DroppedItems.render();
		}

		void ItemManager::update()
		{
			m_DroppedItems.update();
			for (auto item = m_DroppedItems.m_Renderables.begin(); item != m_DroppedItems.m_Renderables.end();)
			{
				if ((*item)->hasExpired())
				{
					//delete (*item);
					//item = m_DroppedItems.m_Renderables.erase(item);

					//expireItem((*item)->getId());
					m_DroppedItemsMap.erase((*item)->getId());
					delete (*item);
					item = m_DroppedItems.m_Renderables.erase(item);
					//return;
				}
				else
					if ((*item)->hasBeenTaken())
					{
						//m_InventoryItems.push_back(*item);
						//auto item_loc = m_Inventory->addItem(*item);
						//(*item)->setDrawDetails(true);
						item = m_DroppedItems.m_Renderables.erase(item);
					}
					else
					{
						++item;
					}
			}
		}

		void ItemManager::setInvEquip(decltype(network::PlayerData::inv_equip) equips)
		{
			for (auto [equip_loc, equip_info] : equips)
			{
				ItemHold* item = new EquipItem( equip_info, 
					ItemDataManager::GetEquip(equip_info.equip_id));
				//m_InventoryItems.push_back(item);
				m_Inventory->addItem(equip_loc, item);
			}
		}

		void ItemManager::setInvUse(decltype(network::PlayerData::inv_use) items)
		{
			for (auto [key, val]: items)
			{
				auto [item_id, item_amount] = val;
				ItemHold* item = new UseItem(item_id, 
					ItemDataManager::Get(item_id), item_amount);
				//m_InventoryItems.push_back(item);
				m_Inventory->addItem(key, item);
			}
		}

		void ItemManager::setEquipsChar(decltype(network::PlayerData::equips_char) equips)
		{
			for (auto [equip_type, equip_info] : equips)
			{
				EquipItem* item = new EquipItem(equip_info, ItemDataManager::GetEquip(equip_info.equip_id));
				m_Equip->addEquip(item);
				CharManager::Instance()->wearItem(equip_type);
			}
		}

		void ItemManager::dropItem(unsigned int item_id, unsigned int item_type_id,
			unsigned int item_kind, maths::vec2 pos, float x_force)
		{
			if (item_kind == network::USE_ITEM)
			{
				const auto item_data = ItemDataManager::Get(item_type_id);
				ItemDrop* temp = new ItemDrop(pos, item_type_id,
					item_data.info.basic_item_info.item_name, item_data.texture_data, m_FootHolds, 
					item_id, x_force);
				m_DroppedItems.add(temp);
				m_DroppedItemsMap.insert(std::make_pair(item_id, temp));
			}
			else
			{
				//Item* temp = new EquipItem(pos, ItemDataManager::GetEquip(item_type_id - 5), m_FootHolds, item_id);
				const auto item_data = ItemDataManager::GetEquip(item_type_id );
				ItemDrop* temp = new ItemDrop(pos, item_type_id,
					item_data.info.item_info.item_name, item_data.icon_texture, m_FootHolds, 
					item_id, x_force);
				m_DroppedItems.add(temp);
				m_DroppedItemsMap.insert(std::make_pair(item_id, temp));
			}
		}

		void ItemManager::addItemToInv(unsigned int item_kind, unsigned int item_loc, unsigned int item_id)
		{
			//if (item_kind == network::EQUIP_ITEM)
			//{
			//	ItemHold* item = new EquipItem(SRL::EquipDbStruct{ item_id, {} },
			//		ItemDataManager::GetEquip(item_id));
			//	//m_InventoryItems.push_back(item);
			//	m_Inventory->addItem(item_loc, item);
			//}
			//else if (item_kind == network::USE_ITEM)
			//{
				ItemHold* item = new UseItem(item_id,
					ItemDataManager::Get(item_id), 1);
				//m_InventoryItems.push_back(item);
				m_Inventory->addItem(item_loc, item);
			//}
		}
		void ItemManager::addEquipItemToInv(SRL::EquipDbStruct item_info, unsigned int item_loc, unsigned int item_id)
		{
			ItemHold* item = new EquipItem(item_info,
				//ItemDataManager::GetEquip(item_id));
				ItemDataManager::GetEquip(item_info.equip_id));
			//m_InventoryItems.push_back(item);
			m_Inventory->addItem(item_loc, item);
		}

		void ItemManager::startExpiring(unsigned int item_id)
		{
			m_DroppedItemsMap[item_id]->startExpire();
			//for (auto item = m_DroppedItems.m_Renderables.begin(); item != m_DroppedItems.m_Renderables.end(); ++item)
			//{

			//	if ((*item)->getId() == item_id)
			//	{
			//		(*item)->startExpire();
			//		//m_DroppedItemsMap.erase(item_id);
			//		//delete (*item);
			//		//item = m_DroppedItems.m_Renderables.erase(item);
			//		return;
			//	}
			//}
		}

		void ItemManager::mapChanged()
		{
			m_DroppedItems.clear();
		}

		void ItemManager::clearItems()
		{
			m_DroppedItems.clear();
			m_DroppedItemsMap.clear();
		}

		ItemDrop * ItemManager::getItem(maths::vec2 pos)
		{
			for (auto item : m_DroppedItems.m_Renderables)
				if (item->isPickedUp())
					return NULL;

			for (auto item = m_DroppedItems.m_Renderables.rbegin(); item != m_DroppedItems.m_Renderables.rend(); ++item)
				if ((*item)->isAbleToPickUp())
					if (pos.Distance((*item)->getBounds().GetBottomMiddle()) < 30)
						return (*item);
			return NULL;
		}
	}
}
