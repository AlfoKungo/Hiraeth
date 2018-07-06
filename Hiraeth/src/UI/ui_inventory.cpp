#include "ui_inventory.h"
#include "item/item.h"

namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, input::Controls control_key, game::CharacterStats *character_stats)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			m_Tabs(new UiTabs<item::Item>()),
			m_CharacterStats(character_stats),
			m_HoldItem(nullptr)
		{

			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(7, 32), graphics::TextureManager::Load("Assets/UI/Inventory/Item.backgrnd3.png")));

			// add tabs graphics - disabled and enabled
			for (int tab_index = SRL::ItemTab::Equip; tab_index <= SRL::ItemTab::Cash; ++tab_index)
			{
				SRL::ItemTab tab = static_cast<SRL::ItemTab>(tab_index);
				m_Tabs->add_tab(tab_index, maths::vec2(9 + 31 * tab, 287), "Inventory", "main", m_BackgroundGroup);
			}
			m_BackgroundGroup->add(m_Tabs);
		}

		void UiInventory::fillGroup()
		{
		}

		void UiInventory::mouse_left_clicked(maths::vec2 mousePos)
		{
			if (mousePos.y < 310 && mousePos.y > 285)
			{
				m_Tabs->mouse_left_clicked(mousePos);
			}
			else
			{
				std::vector<std::unique_ptr<item::Item>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				auto result_item = std::find_if(std::begin(*tab_rends),
					std::end(*tab_rends), [&](auto const& inv_item)
				{ inv_item->setDrawDetails(false);
				return inv_item->getBounds().Contains(mousePos); });
				if (result_item != std::end(*tab_rends))
				{
					m_OldItemPos = (*result_item)->get_position();
					m_HoldItem = (*result_item).get();
					m_IsHolding = true;
					return;
				}
			}
		}

		void UiInventory::mouse_left_released(maths::vec2 mousePos)
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
					std::vector<std::unique_ptr<item::Item>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
					auto result_item = std::find_if(std::begin(*tab_rends),
						std::end(*tab_rends),
						[&](auto const& inv_item)
					{ return (inv_item.get() != m_HoldItem && inv_item->get_position() == newPos); });
					if (result_item != std::end(*tab_rends))
						(*result_item)->setPosition(m_OldItemPos);
				}
				m_HoldItem->setPosition(newPos);
				m_HoldItem = nullptr;
			}
		}

		void UiInventory::mouse_right_clicked(maths::vec2 mousePos)
		{
			std::vector<std::unique_ptr<item::Item>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
			auto result_item = std::find_if(std::begin(*tab_rends),
				std::end(*tab_rends), [&](auto const& inv_item)
			{ inv_item->setDrawDetails(false);
			return inv_item->getBounds().Contains(mousePos); });
			if (result_item != std::end(*tab_rends))
			{
				m_OldItemPos = (*result_item)->get_position();
				std::string item_stats = (*result_item)->getItemStats();
				m_CharacterStats->add_stats(item_stats);
				tab_rends->erase(result_item);
				m_IsHolding = true;
				return;
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
				std::vector<std::unique_ptr<item::Item>> * tab_rends = &m_Tabs->getCurrentTabGroup()->m_TabContent->m_Renderables;
				for (auto & item : *tab_rends)
					item->setDrawDetails(false);
				auto result_item = std::find_if(std::begin(*tab_rends),
					std::end(*tab_rends), [&](auto const& inv_item)
				{ return inv_item->getBounds().Contains(mousePos); });
				if (result_item != std::end(*tab_rends))
				{
					(*result_item)->setDrawDetails(true);
					std::rotate(result_item, result_item + 1, (*tab_rends).end());
					return;
				}
			}
		}

		void UiInventory::addItem(item::Item * new_item)
		{
			UiTab<item::Item> * containing_tab = m_Tabs->getTabByIndex(new_item->getTabType());
			new_item->setPosition(findEmptyPosition(new_item->getTabType()));
			containing_tab->add_data(new_item);
		}

		maths::vec2 UiInventory::findEmptyPosition(unsigned int tab_type)
		{
			UiTab<item::Item> * containing_tab = m_Tabs->getTabByIndex(tab_type);
			for (int index = 0; index < 50; ++index)
			{
				bool flag = false;
				maths::vec2 pos((index % 4) * 36 + 11, 250 - (index / 4) * 35);
				for (auto const & item : containing_tab->m_TabContent->m_Renderables)
				{
					if (item->getBounds().position == pos)
					{
						flag = true;
						break;
					}
				}
				if (!flag)
					return pos;
			}
			return maths::vec2(11, 250);
		}
	}
}
