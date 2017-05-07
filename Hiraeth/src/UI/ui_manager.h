#pragma once

#include <vector>
#include "ui_window.h"
#include "stats.h"
#include "keyboard\keyboard.h"
#include "keyboard\keyboard_event.h"
#include <algorithm>

namespace hiraeth {
	namespace ui {

		class UiManager : public input::MouseEvent, public input::KeyboardEvent
		{
		private:
			std::vector<UiWindow*> m_Windows;
			input::Keyboard* m_Kb;
		public:
			UiManager(input::Keyboard* kb);
			void draw();
			void update();
			void set_keyboard(input::Keyboard* keyboard) { m_Kb = keyboard; }
			void leftButtonClicked(float mx, float my);
			void leftButtonReleased(float mx, float my) const;
			void mouseMove(float pmx, float pmy, float mx, float my) const;
			void ButtonClicked(input::Controls c) override;
			void ButtonReleased(input::Controls c) override;
		private:
			void init_all_windows();
		};

	}
}