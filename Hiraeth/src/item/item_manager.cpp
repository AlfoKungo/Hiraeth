#include "item_manager.h"

namespace hiraeth {
	namespace item {

		ItemManager::ItemManager(const std::vector<physics::FootHold>& foot_holds, 
			ui::UiInventory * inventory, ui::UiEquip * equip)
			:
			m_DroppedItems(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), true),
			m_FootHolds(foot_holds),
			m_Inventory(inventory)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &ItemManager::mapChanged);

			for (int i = 0; i < 12; ++i)
				dropItem(maths::vec2((i - 6) * 80, 0), i % 5);
			dropItem(maths::vec2(0), 0);
			dropItem(maths::vec2(-200, 0), 1);
			dropItem(maths::vec2(200, 0), 2);

			//SRL::ItemData item_data = ItemDataManager::Get(1);
			//equip->addEquip(new Item({ 0,0 }, ItemDataManager::Get(0), m_FootHolds));
			equip->addEquip(new EquipItem({ 0,0 }, ItemDataManager::GetEquip(0), m_FootHolds));
			equip->addEquip(new EquipItem({ 0,0 }, ItemDataManager::GetEquip(2), m_FootHolds));
			equip->addEquip(new EquipItem({ 0,0 }, ItemDataManager::GetEquip(3), m_FootHolds));
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
					delete (*item);
					item = m_DroppedItems.m_Renderables.erase(item);
				}
				else if ((*item)->hasBeenTaken())
				{
					m_InventoryItems.push_back(*item);
					m_Inventory->addItem(*item);
					//(*item)->setDrawDetails(true);
					item = m_DroppedItems.m_Renderables.erase(item);
				}
				else
				{
					++item;
				}
			}
		}

		void ItemManager::dropItem(maths::vec2 pos, unsigned int item_id)
		{
			//SRL::ItemData item_data = ItemDataManager::Get(item_id);
			//m_DroppedItems.add(new Item(pos, ItemDataManager::Get(item_id), m_FootHolds));
			
			if (item_id == 5)
				m_DroppedItems.add(new EquipItem(pos, ItemDataManager::GetEquip(item_id), m_FootHolds));
			else
				m_DroppedItems.add(new UseItem(pos, ItemDataManager::Get(item_id), m_FootHolds));
			
		}

		void ItemManager::mapChanged()
		{
			m_DroppedItems.clear();
		}

		Item * ItemManager::getItem(maths::vec2 pos)
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