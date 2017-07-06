#include "ui_inventory.h"


namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key,const std::vector<item::Item*>& items)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			m_Tab(item::Tab::Equip),
			m_Items(items)
		{

			m_LayerItems.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Equip, new graphics::Group()));
			m_LayerItems.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Use, new graphics::Group()));
			m_LayerItems.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::SetUp, new graphics::Group()));
			m_LayerItems.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Etc, new graphics::Group()));
			m_LayerItems.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Cash, new graphics::Group()));

			m_BackgroundGroup.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(7, 34, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd3.png")));
			m_BackgroundGroup.add(new graphics::Sprite(9, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.0.png")));
			m_BackgroundGroup.add(new graphics::Sprite(40, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.1.png")));
			m_BackgroundGroup.add(new graphics::Sprite(71, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(102, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.3.png")));
			m_BackgroundGroup.add(new graphics::Sprite(133, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.4.png")));

			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(InventoryUpdated, this, &UiInventory::fillLayer);
			fillGroup();
		}

		void UiInventory::fillGroup()
		{
			fillLayer();
			m_ForegroundGroup.clear();
			m_ForegroundGroup.add(m_LayerItems[m_Tab].get());

		}

		void UiInventory::mouse_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				 m_Tab = static_cast<item::Tab>(unsigned int((mousePos.x -11) / 30));
				 fillGroup();
			}

		}
		void UiInventory::fillLayer()
		{
			auto& tab = m_LayerItems[m_Tab];
			tab->clear();
			tab->add(new graphics::Sprite(9 + 31 * m_Tab, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.enabled." + std::to_string(m_Tab) + ".png")));
			int index = 0;
			for (auto inventory_item : m_Items)
			{
				if (inventory_item->getTabType() == m_Tab)
				{
					inventory_item->setPosition(maths::vec2((index % 4) * 37 + 11, 251 - (index / 4) * 35));
					index += 1;
					tab->add(inventory_item);
				}
			}
		}
	}
}
