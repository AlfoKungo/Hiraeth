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
			kb->registerToKey(input::Controls::escape, this);
		}
		void UiManager::draw() const
		{
			m_Layer.render();
			m_MainUi.draw();
		}

		void UiManager::update()
		{
			m_MainUi.update();
			m_Layer.update();
		}

		void UiManager::init_all_windows(input::Keyboard* kb, game::CharacterStats *character_stats)
		{
			m_UiInventory = new UiInventory(maths::vec2(-300, 0), input::Controls::inventory);
			m_Layer.add_ref(m_UiInventory);
			kb->registerToKey(input::Controls::inventory, this);
			m_Layer.add_ref(new UiStats(maths::vec2(0, 0), input::Controls::stats, character_stats));
			kb->registerToKey(input::Controls::stats, this);
			m_Layer.add_ref(new UiSkills(maths::vec2(300, 0), input::Controls::skills));
			kb->registerToKey(input::Controls::skills, this);
			m_Layer.add_ref(new UiQuests(maths::vec2(-600, 0), input::Controls::quests));
			kb->registerToKey(input::Controls::quests, this);
			m_Layer.add_ref(new UiEquip(maths::vec2(500, 0), input::Controls::equip));
			kb->registerToKey(input::Controls::equip, this);
		}

		void UiManager::leftButtonClicked(float mx, float my)
		{
			auto result_window = std::find_if(std::begin(m_Windows), std::end(m_Windows),
				[&](auto const& window) 
			{ return window->is_to_draw && window->isWindowContains(mx, my); });
			if (result_window != m_Windows.end())
			{
				if ((*result_window)->isTitlebarContains(mx, my))
					(*result_window)->attach();
				else
					(*result_window)->mouse_clicked_full((*result_window)->getRelativeLocation(mx, my));
				std::rotate(m_Windows.begin(), result_window, result_window + 1);
			}
		}

		void UiManager::leftButtonReleased(float mx, float my) const
		{
			for (const auto& window : m_Windows)
			{
				window->unattach();
				//if (window->is_holding())
					window->mouse_released_full(maths::vec2(mx, my));
			}
		}

		void UiManager::mouseMove(float pmx, float pmy, float mx, float my) const
		{
			for (const auto& window : m_Windows)
			{
				if (window->is_attached())
					window->move(pmx - mx, pmy - my);
				else
					//if (window->is_holding())
					window->mouse_moved_full(window->getRelativeLocation(mx, my),
						window->getRelativeLocation(pmx, pmy));
			}
		}

		void UiManager::ButtonClicked(input::Controls c)
		{
		if (input::Controls::escape == c)
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
				{ return window->getControlKey() == c; });
				if ((*result_window)->getControlKey() == c)
				{
					(*result_window)->controlKeyClicked();
					if ((*result_window)->is_to_draw)
						std::rotate(m_Windows.begin(), result_window, result_window + 1);
					else
						std::rotate(result_window, result_window + 1, m_Windows.end());
				}
			}
		}

		void UiManager::ButtonReleased(input::Controls c)
		{

		}
	}
}
