#pragma once

#include <vector>
#include "ui_window.h"
#include "stats.h"
#include "keyboard\keyboard.h"
#include <algorithm>

namespace hiraeth {
	namespace ui {

		class UiManager : public input::MouseEvent
		{
		private:
			std::vector<UiWindow*> m_Windows;
			input::Keyboard* m_Kb;
		public:
			UiManager(input::Keyboard* kb, Timer* time);
			void draw();
			void update();
			void set_keyboard(input::Keyboard* keyboard) { m_Kb = keyboard; }
			void leftButtonClicked(float mx, float my);
			void leftButtonReleased(float mx, float my);
			void mouseMove(float pmx, float pmy, float mx, float my);
		private:
			void init_all_windows(Timer* time);
		};

	}
}