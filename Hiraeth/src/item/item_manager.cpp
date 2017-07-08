#include "item_manager.h"

namespace hiraeth {
	namespace item {

		ItemManager::ItemManager(const std::vector<physics::FootHold>& foot_holds, ui::UiInventory * inventory)
			: 
			m_DroppedItems(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), true),
			m_FootHolds(foot_holds),
			m_Inventory(inventory)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &ItemManager::mapChanged);

			serialize_data();
			for (int i = -400; i < 400; i+= 100)
				m_DroppedItems.add(new Item(maths::vec2(i, 0), ItemDataManager::Get((i + 1000) % 3 + 1), m_FootHolds));
			m_DroppedItems.add(new Item(maths::vec2(0),	ItemDataManager::Get(1), m_FootHolds));
			m_DroppedItems.add(new Item(maths::vec2(-200, 0), ItemDataManager::Get(2), m_FootHolds));
			m_DroppedItems.add(new Item(maths::vec2(200, 0), ItemDataManager::Get(3), m_FootHolds));
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
					item = m_DroppedItems.m_Renderables.erase(item);
				}
				else
				{
					++item;
				}
			}
		}

		void ItemManager::dropItem(const char* name, maths::vec2 pos)
		{
			m_DroppedItems.add(new Item(pos, ItemDataManager::Get(3), m_FootHolds));
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

		void ItemManager::serialize_data()
		{

			std::ofstream file("item.data");
			cereal::BinaryOutputArchive oarchive(file);
			//oarchive(int(2));
			file.seekp(sizeof(int) * 4);
			int item_data1_location = file.tellp();
			oarchive(ItemData{"dagger", ItemInfo{Equip}});
			int item_data2_location = file.tellp();
			oarchive(ItemData{"staff", ItemInfo{Equip}});
			int item_data3_location = file.tellp();
			oarchive(ItemData{ "wing", ItemInfo{Use} });
			file.seekp(0);
			oarchive(item_data1_location);
			oarchive(item_data2_location);
			oarchive(item_data3_location);
		}
	}
}