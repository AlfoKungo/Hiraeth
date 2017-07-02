#include "ui_inventory.h"


namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key)
		{
			fillGroup(0);
		}

		void UiInventory::fillGroup(unsigned int selectedTab)
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
			if (selectedTab <= 4 && selectedTab >=0)
				m_Group.add(new graphics::Sprite(9 + 31*selectedTab, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.enabled." + std::to_string(selectedTab) + ".png")));
			//m_Group.add(new graphics::Sprite(120, 120, graphics::TextureManager::Load("Stat.main.BtAuto.normal.1.png")));
		}
		void UiInventory::mouse_clicked(maths::vec2 mousePos)
		{
			//ifc (my )
			unsigned int newTab;
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				 newTab = unsigned int((mousePos.x -11) / 30);
				 fillGroup(newTab);
			}

		}
	}
}
