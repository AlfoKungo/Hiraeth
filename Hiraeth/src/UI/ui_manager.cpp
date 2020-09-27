#include "ui_manager.h"

namespace hiraeth {
	namespace ui {
		class UiKeyConfig;

		UiManager::UiManager(input::Keyboard* kb)
			: m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag")),
			m_Windows(m_Layer.m_RefRenderables),
			m_Kb(kb),
			m_MainUi(kb)
		{
			init_all_windows(kb, m_MainUi.getCharacterStats());
			kb->registerToMouse(this);
			kb->registerToKey(GLFW_KEY_ESCAPE, escape, this);
			//EventManager *m_EventManager = EventManager::Instance();
			//m_EventManager->createEvent<SRL::EquipItemType>(ItemWore);
		}
		void UiManager::draw() const
		{
			m_MainUi.draw();
			m_Layer.render();
		}

		void UiManager::update()
		{
			m_MainUi.update();
			m_Layer.update();
		}

		void UiManager::init_all_windows(input::Keyboard* kb, game::CharacterStats *character_stats)
		{
			m_UiInventory = new UiInventory(maths::vec2(-300, 0), inventory, character_stats);
			m_Layer.add_ref(m_UiInventory);
			kb->registerToKey(GLFW_KEY_I, inventory, this);

			m_UiStats = new UiStats(maths::vec2(0, 0), stats, character_stats);
			m_Layer.add_ref(m_UiStats);
			kb->registerToKey(GLFW_KEY_S, stats, this);

			m_UiKeyConfig = new UiKeyConfig(maths::vec2(-700, 0), key_config, kb);
			m_Layer.add_ref(m_UiKeyConfig);
			kb->registerToKey(GLFW_KEY_TAB, key_config, this);

			m_UiSkills = new UiSkills(maths::vec2(300, 0), skills, m_UiKeyConfig);
			m_Layer.add_ref(m_UiSkills);
			kb->registerToKey(GLFW_KEY_K, skills, this);

			m_UiQuests = new UiQuests(maths::vec2(-600, 0), quests);
			m_Layer.add_ref(m_UiQuests);
			kb->registerToKey(GLFW_KEY_Q, quests, this);

			m_UiEquip = new UiEquip(maths::vec2(500, 0), equip, character_stats);
			m_Layer.add_ref(m_UiEquip);
			kb->registerToKey(GLFW_KEY_E, equip, this);

			m_UiParty = new UiParty(maths::vec2(-300, -100));
			m_Layer.add_ref(m_UiParty);
			kb->registerToKey(GLFW_KEY_P, party, this);

			m_UiTrade = new UiTrade(maths::vec2(-300, -100));
			m_Layer.add_ref(m_UiTrade);
			//m_UiTrade->setEquips(m_UiEquip->getEquips());

			m_UiShop = new UiShop(maths::vec2(-300, -100));
			m_Layer.add_ref(m_UiShop);

			m_UiInventory->setTradeUi(&m_UiTrade->m_Group.m_Pos, &m_UiTrade->m_IsTrading);
			//m_UiTrade->is_to_draw = true;
		}

		bool UiManager::leftButtonClicked(float mx, float my)
		{
			const auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
				[&](auto const& window) 
			{ return window->is_to_draw && window->isWindowContains(maths::vec2{ mx, my }); });
			if (result_window != m_Windows.end())
			{
				if ((*result_window)->isTitlebarContains(mx, my))
					(*result_window)->attach();
				else
					(*result_window)->mouse_left_clicked_full({ mx, my });
				std::rotate(m_Windows.begin(), result_window, result_window + 1);
				return true;
			}
			return false;
		}

		bool UiManager::leftButtonReleased(float mx, float my) 
		{
			for (const auto& window : m_Windows)
			{
				window->unattach();
					window->mouse_left_released_full(maths::vec2(mx, my));
			}
			return false;
		}

		bool UiManager::rightButtonClicked(float mx, float my)
		{
			auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
				[&](auto const& window) 
			{ return window->is_to_draw && window->isWindowContains(maths::vec2{ mx, my }); });
			if (result_window != m_Windows.end())
			{
				switch ((*result_window)->m_ControlKey)
				{
				case (UiKey::equip):
				{
					auto equip_to_move_to_inventory = m_UiEquip->unEquip((*result_window)->getRelativeLocation(mx, my));
					if (equip_to_move_to_inventory != nullptr)
					{
						auto pos_index = m_UiInventory->addItem(equip_to_move_to_inventory); 
						// delete this and move this to be a network interaction
						//EventManager *m_EventManager = EventManager::Instance();
						//m_EventManager->execute<SRL::EquipItemType>(ItemUnWore, equip_to_move_to_inventory->getItemType());
						CharManager::Instance()->unWearItem(equip_to_move_to_inventory->getItemType());
						NetworkManager::Instance()->sendWearItem(equip_to_move_to_inventory->getItemType(), pos_index);
					}
					std::rotate(m_Windows.begin(), result_window, result_window + 1);
					break;
				}
				case (UiKey::inventory):
				{
					//auto equip_to_wear = m_UiInventory->getEquipItem((*result_window)->getRelativeLocation(mx, my));
					auto [index, equip_to_wear] = m_UiInventory->itemClickedOn((*result_window)->getRelativeLocation(mx, my));
					if (equip_to_wear != nullptr)
					{
						//maths::vec2 old_item_pos = equip_to_wear->getPosition();
						auto equip_to_return_to_inventory = m_UiEquip->addEquip(equip_to_wear);
						//EventManager *m_EventManager = EventManager::Instance();
						//m_EventManager->execute<SRL::EquipItemType>(ItemWore, equip_to_wear->getItemType());
						CharManager::Instance()->wearItem(equip_to_wear->getItemType());
						NetworkManager::Instance()->sendWearItem(equip_to_wear->getItemType(), index);
						if (equip_to_return_to_inventory != nullptr)
							//m_UiInventory->addItem(equip_to_return_to_inventory, old_item_pos);
							m_UiInventory->addItem(index,equip_to_return_to_inventory );
					}
					std::rotate(m_Windows.begin(), result_window, result_window + 1);
					break;
				}
				default:
					if (!(*result_window)->isTitlebarContains(mx, my))
						(*result_window)->mouse_right_clicked_full({ mx, my });
					std::rotate(m_Windows.begin(), result_window, result_window + 1);
					break;
				}
				return true;
			}
			return false;
		}

		bool UiManager::mouseMove(float pmx, float pmy, float mx, float my) 
		{
			for (const auto& window : m_Windows)
			{
				if (window->is_attached())
					window->move(pmx - mx, pmy - my);
				else
					if (window->is_to_draw && window->isWindowContains({ mx, my }))
					{
						window->mouse_moved_full({ mx, my }, { pmx, pmy });
						return true;
					}
			}
			return false;
		}

		bool UiManager::is_window_contains(maths::vec2 mouse_pos) const
		{
			auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
				[&](auto const& window) 
			{ return window->is_to_draw && window->isWindowContains(mouse_pos); });
			return result_window != m_Windows.end();
		}

		void UiManager::ButtonClicked(input::Key key)
		{
			if (escape == key)
			{
				auto window = m_Windows.begin();
				if (*window == m_UiTrade)
					return;
				(*window)->is_to_draw = false;
				(*window)->is_to_update = false;

				std::rotate(window, window + 1, m_Windows.end());
			}
			else
			{
				auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
					[&](auto const& window)
				{ return window->getControlKey() == key; });
				if ((*result_window)->getControlKey() == key)
				{
					(*result_window)->controlKeyClicked();
					if ((*result_window)->is_to_draw)
						std::rotate(m_Windows.begin(), result_window, result_window + 1);
					else
						std::rotate(result_window, result_window + 1, m_Windows.end());
				}
			}
		}

		void UiManager::ButtonReleased(input::Key key)
		{

		}
	}
}
