#include "ui_inventory.h"
#include "item/item.h"

namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			m_Tabs(&m_ForegroundGroup),
			m_HoldItem(nullptr)
		{

			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd2.png")));
			m_BackgroundGroup.add(new graphics::Sprite(maths::vec2(7, 32), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd3.png")));

			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::ItemTab::Equip; tab_index <= SRL::ItemTab::Cash; ++tab_index)
			{
				SRL::ItemTab tab = static_cast<SRL::ItemTab>(tab_index);
				m_Tabs.add_tab(tab_index, maths::vec2(9 + 31 * tab, 287), "Inventory", "main", &m_BackgroundGroup);
			}
		}

		void UiInventory::fillGroup()
		{
		}

		void UiInventory::mouse_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				m_Tabs.mouse_clicked(mousePos);
			}
			else
			{
				graphics::Group * currentTab = m_Tabs.getCurrentTabGroup();
				auto result_item = std::find_if(std::begin(currentTab->m_Renderables),
					std::end(currentTab->m_Renderables),[&](auto const& inv_item)
				{ return inv_item->getBounds().Contains(mousePos); });
				if (result_item != std::end(currentTab->m_Renderables))
				{
					m_HoldItem = dynamic_cast<item::Item *>((*result_item));
					std::rotate(result_item, result_item + 1, currentTab->m_Renderables.end());
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
				maths::vec2 item_pos = m_HoldItem->getBounds().GetMiddle();
				// set new location according to item's center
				const maths::vec2 newPos = ((unsigned int((item_pos.x - 11) / 36) >= 4) || ((unsigned int(item_pos.y - 75) / 35) >= 6))
					? m_OldItemPos :
					maths::vec2((unsigned int((item_pos.x - 11) / 36) % 4) * 36 + 11, (unsigned int(item_pos.y - 75) / 35) % 6 * 35 + 75);
				if (newPos != m_OldItemPos)
				{
					graphics::Group * currentTab = m_Tabs.getCurrentTabGroup();
					auto result_item = std::find_if(std::begin(currentTab->m_Renderables),
						std::end(currentTab->m_Renderables),
						[&](auto const& inv_item)
					{ return (inv_item != m_HoldItem && inv_item->getPosition() == newPos); });
					if (result_item != std::end(currentTab->m_Renderables))
						(*result_item)->setPosition(m_OldItemPos);
				}
				m_HoldItem->setPosition(newPos);
				m_HoldItem = nullptr;
			}
		}

		void UiInventory::mouse_moved(float mx, float my, maths::vec2 mousePos)
		{
			if (m_HoldItem != nullptr)
			{
				m_HoldItem->setPosition(m_HoldItem->getBounds().position + maths::vec2(-mx, my));
			}
			else
			{
				if (!(mousePos.y < 310 && mousePos.y > 285))
				{
					m_Tabs.mouse_clicked(mousePos);
					graphics::Group * currentTab = m_Tabs.getCurrentTabGroup();
					auto result_item = std::find_if(std::begin(currentTab->m_Renderables),
						std::end(currentTab->m_Renderables), [&](auto const& inv_item)
					{ return inv_item->getBounds().Contains(mousePos); });
					if (result_item != std::end(currentTab->m_Renderables))
					{
						item::Item * found_item = dynamic_cast<item::Item *>((*result_item));
						found_item->setDrawDetails(true);
						return;
					}
				}
			}
		}

		void UiInventory::addItem(item::Item * new_item)
		{
			graphics::Group * containing_tab = m_Tabs.getTabByIndex(new_item->getTabType());
			size_t index = containing_tab->m_Renderables.size() - 1;
			new_item->setPosition(maths::vec2((index % 4) * 36 + 11, 250 - (index / 4) * 35));
			containing_tab->add(new_item);
		}
	}
}
