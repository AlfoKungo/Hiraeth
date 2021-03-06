#include "ui_manager.h"
#include "ui_inventory.h"

namespace hiraeth {
	namespace ui {

		UiManager::UiManager(input::Keyboard* kb)
			: m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag")),
			m_Windows(m_Layer.m_RefRenderables),
			m_Kb(kb),
			m_MainUi()
		{
			init_all_windows(kb, m_MainUi.getCharacterStats());
			kb->registerToMouse(this);
			kb->registerToKey(GLFW_KEY_ESCAPE, escape, this);
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
			kb->registerToKey(GLFW_KEY_A, inventory, this);

			m_Layer.add_ref(new UiStats(maths::vec2(0, 0), stats, character_stats));
			kb->registerToKey(GLFW_KEY_S, stats, this);

			m_UiSkills = new UiSkills(maths::vec2(300, 0), skills);
			m_Layer.add_ref(m_UiSkills);
			kb->registerToKey(GLFW_KEY_D, skills, this);

			m_Layer.add_ref(new UiQuests(maths::vec2(-600, 0), quests));
			kb->registerToKey(GLFW_KEY_Q, quests, this);

			m_UiEquip = new UiEquip(maths::vec2(500, 0), equip);
			m_Layer.add_ref(m_UiEquip);
			kb->registerToKey(GLFW_KEY_E, equip, this);
			//m_UiEquip->addEquip()
		}

		bool UiManager::leftButtonClicked(float mx, float my)
		{
			auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
				[&](auto const& window) 
			{ return window->is_to_draw && window->isWindowContains(maths::vec2{ mx, my }); });
			if (result_window != m_Windows.end())
			{
				if ((*result_window)->isTitlebarContains(mx, my))
					(*result_window)->attach();
				else
					(*result_window)->mouse_left_clicked_full((*result_window)->getRelativeLocation(mx, my));
				std::rotate(m_Windows.begin(), result_window, result_window + 1);
				return true;
			}
			return false;
		}

		bool UiManager::leftButtonReleased(float mx, float my) const
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
			for (const auto& window : m_Windows)
			{
				window->mouse_right_clicked_full(maths::vec2(mx, my));
			}
			return false;
		}

		bool UiManager::mouseMove(float pmx, float pmy, float mx, float my) const
		{
			for (const auto& window : m_Windows)
			{
				if (window->is_attached())
					window->move(pmx - mx, pmy - my);
				else
					window->mouse_moved_full(window->getRelativeLocation(mx, my),
						window->getRelativeLocation(pmx, pmy));
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
