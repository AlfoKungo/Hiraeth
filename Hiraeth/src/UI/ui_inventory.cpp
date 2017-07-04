#include "ui_inventory.h"


namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key,const std::vector<item::Item*>& items)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			m_Tab(Equip),
			m_Items(items)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(InventoryUpdated, this, &UiInventory::fillGroup);
			fillGroup();
		}

		void UiInventory::fillGroup()
		{
			m_Group.clear();
			m_Group.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd.png")));
			m_Group.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd2.png")));
			m_Group.add(new graphics::Sprite(7, 34, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd3.png")));
			m_Group.add(new graphics::Sprite(9, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.0.png")));
			m_Group.add(new graphics::Sprite(40, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.1.png")));
			m_Group.add(new graphics::Sprite(71, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.2.png")));
			m_Group.add(new graphics::Sprite(102, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.3.png")));
			m_Group.add(new graphics::Sprite(133, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.4.png")));
			if (m_Tab <= 4 && m_Tab >=0)
				m_Group.add(new graphics::Sprite(9 + 31*m_Tab, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.enabled." + std::to_string(m_Tab) + ".png")));
			int x = 11;
			int index = 0;
			for (auto inventory_item : m_Items)
			{
				inventory_item->setPosition(maths::vec2((index % 4) * 37 + 11, 251 - (index / 4)* 35));
				index += 1;
				m_Group.add(inventory_item);
			}
		}
		void UiInventory::mouse_clicked(maths::vec2 mousePos)
		{
			unsigned int newTab;
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				 m_Tab = static_cast<Tab>(unsigned int((mousePos.x -11) / 30));
				 fillGroup();
			}

		}
	}
}
