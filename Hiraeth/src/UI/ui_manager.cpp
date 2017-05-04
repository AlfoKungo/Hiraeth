#include "ui_manager.h"

namespace hiraeth {
	namespace ui {

		UiManager::UiManager(input::Keyboard* kb, Timer* time)
			: m_Kb(kb)
		{
			init_all_windows(time);
		}
		void UiManager::draw()
		{
			//for (UiWindow* window : m_Windows)
			for (std::vector<UiWindow*>::reverse_iterator window = m_Windows.rbegin(); window != m_Windows.rend(); ++window)
			{
				if ((*window)->is_to_draw)
					(*window)->draw();
			}
		}

		void UiManager::update()
		{
			for (UiWindow* window : m_Windows)
			{
				if (window->is_to_update)
					window->update();
			}
			double x, y;
		}

		void UiManager::init_all_windows(Timer* time)
		{
			Stats* s = new Stats(maths::vec2(0,0), time);
			m_Kb->registerToKey(83, s);
			m_Windows.push_back(s);
			Stats* s2 = new Stats(maths::vec2(300,0), time);
			m_Kb->registerToKey(69, s2);
			m_Windows.push_back(s2);
		}
		
		void UiManager::leftButtonClicked(float mx, float my)
		{
			//for (UiWindow* window : m_Windows)
			//for (auto const& window : m_Windows)
			for (std::vector<UiWindow*>::iterator window = m_Windows.begin(); window != m_Windows.end(); ++window)
			{
				if ((*window)->isWindowContains(mx, my))
				{
					if ((*window)->isTitlebarContains(mx, my))
					{
						(*window)->attach();
						std::iter_swap(m_Windows.begin(), window);
						break;
					}
					else
					{
						(*window)->mouse_clicked();
					}
				}
			}
		}
		
		void UiManager::leftButtonReleased(float mx, float my)
		{
			for (UiWindow* window : m_Windows)
			{
				window->unattach();
			}
		}

		void UiManager::mouseMove(float pmx, float pmy, float mx, float my)
		{
			for (UiWindow* window : m_Windows)
			{
				if (window->is_attached())
					window->move(pmx - mx, pmy - my);
			}
		}
	}
}
