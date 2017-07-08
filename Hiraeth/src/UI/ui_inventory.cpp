#include "ui_inventory.h"
#include "item/item.h"

namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			m_Tab(item::Tab::Equip),
			m_HoldItem(nullptr)
		{

			m_Items.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Equip, new graphics::Group()));
			m_Items.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Use, new graphics::Group()));
			m_Items.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::SetUp, new graphics::Group()));
			m_Items.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Etc, new graphics::Group()));
			m_Items.insert(std::pair<item::Tab, std::unique_ptr<graphics::Group>>(item::Tab::Cash, new graphics::Group()));

			m_BackgroundGroup.add(new graphics::Sprite(0, 0, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(6, 5, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(7, 34, graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd3.png")));
			m_BackgroundGroup.add(new graphics::Sprite(9, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.0.png")));
			m_BackgroundGroup.add(new graphics::Sprite(40, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.1.png")));
			m_BackgroundGroup.add(new graphics::Sprite(71, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(102, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.3.png")));
			m_BackgroundGroup.add(new graphics::Sprite(133, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.disabled.4.png")));

			fillGroup();
			for (int tab_index = item::Tab::Equip; tab_index != item::Tab::Cash + 1; ++tab_index)
			{
				item::Tab tab = static_cast<item::Tab>(tab_index);
				m_Items[tab]->add(new graphics::Sprite(9 + 31 * tab, 289, graphics::TextureManager::Load("Assets/UI/Inventory/Tab.enabled." + std::to_string(tab) + ".png")));
			}
		}

		void UiInventory::fillGroup()
		{
			m_ForegroundGroup.clear();
			m_ForegroundGroup.add(m_Items[m_Tab].get());
		}

		void UiInventory::mouse_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				 m_Tab = static_cast<item::Tab>(unsigned int((mousePos.x -11) / 30));
				 fillGroup();
			}
			else
			{
				auto result_item = std::find_if(std::begin(m_Items[m_Tab]->m_Renderables),
					std::end(m_Items[m_Tab]->m_Renderables),[&](auto const& inv_item)
				{ return inv_item->getBounds().Contains(mousePos); });
				if (result_item != std::end(m_Items[m_Tab]->m_Renderables))
				{
					m_HoldItem = dynamic_cast<item::Item *>((*result_item));
					std::rotate(result_item, result_item + 1, m_Items[m_Tab]->m_Renderables.end());
					m_OldItemPos = m_HoldItem->getPosition();
					m_IsHolding = true;
					return;
				}
			}
		}

		void UiInventory::mouse_released(maths::vec2 mousePos)
		{
			if (m_HoldItem != nullptr)
			{
				m_IsHolding = false;
				maths::vec2 pos(getRelativeLocation(mousePos.x, mousePos.y));
				const maths::vec2 newPos = ((unsigned int((pos.x - 11) / 36) >= 4) || ((unsigned int(pos.y - 75) / 35) >= 6))
					? m_OldItemPos :
					maths::vec2((unsigned int((pos.x - 11) / 36) % 4) * 36 + 11, (unsigned int(pos.y - 75) / 35) % 6 * 35 + 75);
				if (newPos != m_OldItemPos)
				{
					auto result_item = std::find_if(std::begin(m_Items[m_Tab]->m_Renderables),
						std::end(m_Items[m_Tab]->m_Renderables),
						[&](auto const& inv_item)
					{ return (inv_item != m_HoldItem && inv_item->getPosition() == newPos); });
					if (result_item != std::end(m_Items[m_Tab]->m_Renderables))
						(*result_item)->setPosition(m_OldItemPos);
				}
				m_HoldItem->setPosition(newPos);
				m_HoldItem = nullptr;
			}
		}

		void UiInventory::mouse_moved(float mx, float my)
		{
			if (m_HoldItem != nullptr)
			{
				m_HoldItem->setPosition(m_HoldItem->getBounds().position + maths::vec2(-mx, my));
			}
		}

		void UiInventory::addItem(item::Item * new_item)
		{
			size_t index = m_Items[new_item->getTabType()]->m_Renderables.size() - 1;
			new_item->setPosition(maths::vec2((index % 4) * 36 + 11, 250 - (index / 4) * 35));
			m_Items[new_item->getTabType()]->add(new_item);
		}
	}
}
