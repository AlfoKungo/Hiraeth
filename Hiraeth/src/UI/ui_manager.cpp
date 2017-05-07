#include "ui_manager.h"

namespace hiraeth {
	namespace ui {

		UiManager::UiManager(input::Keyboard* kb)
			: m_Kb(kb)
		{
			init_all_windows();
			kb->registerToKey(input::Controls::escape, this);
			kb->registerToKey(input::Controls::stats_a, this);
			kb->registerToKey(input::Controls::stats_b, this);
			kb->registerToKey(input::Controls::stats_c, this);
		}
		void UiManager::draw()
		{
			for (auto window = m_Windows.rbegin(); window != m_Windows.rend(); ++window)
			{
				if ((*window)->is_to_draw)
					(*window)->draw();
			}
		}

		void UiManager::update()
		{
			for (const auto & window : m_Windows)
			{
				if (window->is_to_update)
					window->update();
			}
		}

		void UiManager::init_all_windows()
		{
			m_Windows.push_back(new Stats(maths::vec2(0, 0), input::Controls::stats_a));
			m_Windows.push_back(new Stats(maths::vec2(-300, 0), input::Controls::stats_b));
			m_Windows.push_back(new Stats(maths::vec2(300, 0), input::Controls::stats_c));
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
						//if ((*window)->is_to_draw)
							std::rotate(m_Windows.begin(), window, window + 1);
						//else
							//std::rotate(window, window + 1, m_Windows.end());
					}
				}
			}
		}

		void UiManager::ButtonReleased(input::Controls c)
		{

		}
	}
}
