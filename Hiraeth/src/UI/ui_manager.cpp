#include "ui_manager.h"

namespace hiraeth {
	namespace ui {

		UiManager::UiManager(input::Keyboard* kb, game::CharacterStats *character_stats)
			: m_Layer(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag")),
			//: m_Layer(shader),
			m_Windows(m_Layer.m_RefRenderables),
			m_Kb(kb),
			m_StatsA(maths::vec2(0, 0), input::Controls::stats_a, character_stats),
			m_StatsB(maths::vec2(-300, 0), input::Controls::stats_b, character_stats),
			m_StatsC(maths::vec2(300, 0), input::Controls::stats_c, character_stats)
		{
			init_all_windows();
			kb->registerToMouse(this);
			kb->registerToKey(input::Controls::escape, this);
			kb->registerToKey(input::Controls::stats_a, this);
			kb->registerToKey(input::Controls::stats_b, this);
			kb->registerToKey(input::Controls::stats_c, this);
		}
		void UiManager::draw()
		{
			m_Layer.render();
			//m_Layer_b.render();
			//m_Layer_c.render();
		}

		void UiManager::update()
		{
			m_Layer.update();
			//m_Layer_b.update();
			//m_Layer_c.update();
		}

		void UiManager::init_all_windows()
		{
			m_Layer.add_ref(&m_StatsA);
			m_Layer.add_ref(&m_StatsB);
			m_Layer.add_ref(&m_StatsC);
			//m_Layer_b.add(new Stats(maths::vec2(-300, 0), input::Controls::stats_b));
			//m_Layer_c.add(new Stats(maths::vec2(300, 0), input::Controls::stats_c));
		}

		void UiManager::leftButtonClicked(float mx, float my)
		{
			//for (auto & window : m_Windows)

			for (auto window = m_Windows.begin(); window != m_Windows.end(); ++window)
			{
				if ((*window)->isWindowContains(mx, my))
				{
					if ((*window)->isTitlebarContains(mx, my))
					{
						(*window)->attach();
					}
					else
					{
						(*window)->mouse_clicked();
					}
					std::rotate(m_Windows.begin(), window, window + 1);
					break;
				}
			}
		}

		void UiManager::leftButtonReleased(float mx, float my) const
		{
			for (const auto& window : m_Windows)
			{
				window->unattach();
			}
		}

		void UiManager::mouseMove(float pmx, float pmy, float mx, float my) const
		{
			for (const auto& window : m_Windows)
			{
				if (window->is_attached())
					window->move(pmx - mx, pmy - my);
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
				for (auto window = m_Windows.begin(); window != m_Windows.end(); ++window)
				{
					if ((*window)->getControlKey() == c)
					{
						(*window)->controlKeyClicked();
						if ((*window)->is_to_draw)
							std::rotate(m_Windows.begin(), window, window + 1);
						else
							std::rotate(window, window + 1, m_Windows.end());
						break;
					}
				}
			}
		}

		void UiManager::ButtonReleased(input::Controls c)
		{

		}
	}
}
