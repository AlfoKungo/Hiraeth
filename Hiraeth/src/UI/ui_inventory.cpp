#include "ui_inventory.h"
#include "item/use_item.h"
#include "basic/network_handler.h"

namespace hiraeth {
	namespace ui {
		UiInventory::UiInventory(maths::vec2 pos, UiKey control_key, game::CharacterStats *character_stats)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 172, 335), control_key),
			//m_Tabs(new UiTabs<item::Item>()),
			m_Tabs(new UiTabs<UiTabInventory>()),
			m_CharacterStats(character_stats),
			m_HoldItem(0, nullptr)
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

			//EventManager *m_EventManager = EventManager::Instance();
			//m_EventManager->createEvent<unsigned int, unsigned int, unsigned int>(SwitchInventoryItems);
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
				auto rends = &m_Tabs->getCurrentTabGroup()->m_MtGroup->m_Renderables;
				//auto tab = m_Tabs->getCurrentTabGroup();
				//const auto new_pos_index = tab->getPosIndexByMousePos(mousePos);
				//auto item = rends->find(new_pos_index);
			auto item = getItemPair(mousePos);
				if (item != rends->end())
				{
					item->second->setDrawDetails(false);
					m_HoldItem = std::make_pair(item->first, item->second.get());
					m_IsHolding = true;
				}
			}
		}

		void UiInventory::mouse_left_released(maths::vec2 mousePos)
		{
			if (m_HoldItem.second != nullptr)
			{
				m_IsHolding = false;
				auto tab = m_Tabs->getCurrentTabGroup();
				auto& rends = tab->m_MtGroup->m_Renderables;
				auto new_pos_index = tab->getPosIndexByMousePos(mousePos);
				if (new_pos_index != m_HoldItem.first)
				{
					auto item2 = rends.find(new_pos_index);
					//if ((item1 != rends.end()) && (item2 != rends.end()))
					if (item2 != rends.end())
					{
						auto item1 = rends.find(m_HoldItem.first);
						std::swap(item1->second, item2->second);
						item1->second->setPosition(UiTabInventory::getPosByLocIndex(m_HoldItem.first));
						item2->second->setPosition(UiTabInventory::getPosByLocIndex(new_pos_index));
					}
					else
					{
						tab->changeItemPos(m_HoldItem.first, new_pos_index);
					}
					NetworkManager::Instance()->sendSwitchInventoryItems(m_HoldItem.first,
						new_pos_index, m_Tabs->getTabIndex());
					//EventManager* m_EventManager = EventManager::Instance();
					//m_EventManager->execute(SwitchInventoryItems, m_HoldItem.first, new_pos_index, m_Tabs->getTabIndex());
				}
				else
				{
					m_HoldItem.second->setPosition(tab->getPosByLocIndex(new_pos_index));
				}
				m_HoldItem = std::make_pair(0, nullptr);
			}
		}

		void UiInventory::mouse_right_clicked(maths::vec2 mousePos)
		{
		}

		void UiInventory::mouse_moved(float mx, float my, maths::vec2 mousePos)
		{
			if (m_HoldItem.second != nullptr)
			{
				m_HoldItem.second->setPosition(m_HoldItem.second->getBounds().position + maths::vec2(-mx, my));
			}
			else
			{
				auto tab_rends = &m_Tabs->getCurrentTabGroup()->m_MtGroup->m_Renderables;
				for (auto & item : *tab_rends)
					item.second->setDrawDetails(false);
				auto pos_index = m_Tabs->getCurrentTabGroup()->getPosIndexByMousePos(mousePos);
				auto item = tab_rends->find(pos_index);
				if (item != tab_rends->end())
					(item->second)->setDrawDetails(true);
			}
		}

		unsigned int UiInventory::addItem(item::ItemHold* new_item)
		{
			auto containing_tab = m_Tabs->getTabByIndex(new_item->getTabType());
			auto pos_index = containing_tab->add_data_b(new_item);
			return pos_index;
		}

		void UiInventory::addItem(unsigned int pos_index, item::ItemHold* new_item)
		{
			auto containing_tab = m_Tabs->getTabByIndex(new_item->getTabType());
			auto& rends = containing_tab->m_MtGroup->m_Renderables;
			if (rends.find(pos_index) != rends.end())
			{
				auto use_item = dynamic_cast<item::UseItem*>(rends.find(pos_index)->second.get());
				use_item->increaseAmount(1);
				delete new_item;
			}
			else
			{
				containing_tab->add_data(pos_index, new_item);
			}
		}

		void UiInventory::use_item(maths::vec2 mousePos)
		{
			auto rends = &m_Tabs->getCurrentTabGroup()->m_MtGroup->m_Renderables;
			//auto tab = m_Tabs->getCurrentTabGroup();
			//const auto new_pos_index = tab->getPosIndexByMousePos(mousePos);
			//auto item = rends->find(new_pos_index);
			auto item = getItemPair(mousePos);
			if (item != rends->end())
			{
				//item->second->setDrawDetails(false);
				auto use_item = dynamic_cast<item::UseItem*>(item->second.get());
				SRL::ItemPropertiesMap* item_stats = use_item->getItemProperties();
				if (m_CharacterStats->activateUseItem(item_stats))
				{
					if (use_item->getAmount() > 1)
						use_item->increaseAmount(-1);
					else
						rends->erase(item);
				}
			}
			
			//auto result_item = std::find_if(std::begin(*tab_rends),
			//	std::end(*tab_rends), [&](auto const& inv_item)
			//{
			//	return inv_item.second->getBounds().Contains(mousePos); });
			//if (result_item != std::end(*tab_rends))
			//{
			//	auto use_item = dynamic_cast<item::UseItem*>(result_item->second.get());
			//	//m_OldItemPos = use_item->getPosition();
			//	SRL::ItemPropertiesMap* item_stats = use_item->getItemProperties();
			//	if (m_CharacterStats->activateUseItem(item_stats))
			//	{
			//		tab_rends->erase(result_item);
			//		m_IsHolding = true;
			//	}
			//}
		}

		std::pair<unsigned int, item::EquipItem*> UiInventory::getEquipItem(maths::vec2 mousePos)
		{
			auto rends = &m_Tabs->getCurrentTabGroup()->m_MtGroup->m_Renderables;
			//auto tab = m_Tabs->getCurrentTabGroup();
			//const auto new_pos_index = tab->getPosIndexByMousePos(mousePos);
			//auto item = rends->find(new_pos_index);
			auto item = getItemPair(mousePos);
			if (item != rends->end())
			{
				item->second->setDrawDetails(false);
				auto index = item->first;
				auto* equip_item = dynamic_cast<item::EquipItem*>(item->second.release());
				rends->erase(item->first);
				return std::make_pair(index, equip_item);
			}

			return std::make_pair(0, nullptr);
		}

		unsigned int UiInventory::findEmptyPosition(unsigned int tab_type)
		{
			auto containing_tab = m_Tabs->getTabByIndex(tab_type);
			return containing_tab->findAvailableLoc();
		}

		std::map<unsigned, std::unique_ptr<item::ItemHold>>::iterator UiInventory::getItemPair(maths::vec2 mouse_pos)
		{
			auto rends = &m_Tabs->getCurrentTabGroup()->m_MtGroup->m_Renderables;
			auto tab = m_Tabs->getCurrentTabGroup();
			const auto new_pos_index = tab->getPosIndexByMousePos(mouse_pos);
			return rends->find(new_pos_index);
		}

		std::pair<unsigned int, item::EquipItem*> UiInventory::itemClickedOn(maths::vec2 mousePos)
		{
			switch (m_Tabs->getTabIndex())
			{
			case 0:
				return getEquipItem(mousePos);
			case 1:
				use_item(mousePos);
				return std::make_pair(0, nullptr);
			default:
				return std::make_pair(0, nullptr);
			}

		}
	}
}
