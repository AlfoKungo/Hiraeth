#include "ui_inventory.h"


namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key)
		{
			fillGroup();
		}

		void UiInventory::fillGroup()
		{
			m_Group.clear();
			m_Group.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Assets/Inventory/Item.backgrnd.png")));
			m_Group.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Assets/Inventory/Item.backgrnd2.png")));
			m_Group.add(new graphics::Sprite(7, 34, graphics::TextureManager::Load("Assets/Inventory/Item.backgrnd3.png")));
			m_Group.add(new graphics::Sprite(9, 289, graphics::TextureManager::Load("Assets/Inventory/Tab.disabled.0.png")));
			m_Group.add(new graphics::Sprite(40, 289, graphics::TextureManager::Load("Assets/Inventory/Tab.disabled.1.png")));
			m_Group.add(new graphics::Sprite(70, 289, graphics::TextureManager::Load("Assets/Inventory/Tab.enabled.2.png")));
			m_Group.add(new graphics::Sprite(102, 289, graphics::TextureManager::Load("Assets/Inventory/Tab.disabled.3.png")));
			m_Group.add(new graphics::Sprite(133, 289, graphics::TextureManager::Load("Assets/Inventory/Tab.disabled.4.png")));
			//m_Group.add(new graphics::Sprite(120, 120, graphics::TextureManager::Load("Stat.main.BtAuto.normal.1.png")));
		}
	}
}
