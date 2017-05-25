#pragma once

#include <vector>
#include <algorithm>

#include "ui_window.h"
#include "ui_stats.h"
#include "ui_inventory.h"
#include "main_ui.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"
#include "game/character_stats.h"

namespace hiraeth {
	namespace ui {

		class UiManager : public input::MouseEvent, public input::KeyboardEvent
		{
		private:
			graphics::Layer<UiWindow> m_Layer;
			std::vector<UiWindow*>& m_Windows;
			input::Keyboard* m_Kb;
			MainUi m_MainUi;
		public:
			UiManager(input::Keyboard* kb, game::CharacterStats *character_stats);
			void update();
			void draw() const;
			void set_keyboard(input::Keyboard* keyboard) { m_Kb = keyboard; }
			void leftButtonClicked(float mx, float my) override;
			void leftButtonReleased(float mx, float my) const override;
			void mouseMove(float pmx, float pmy, float mx, float my) const override;
			void ButtonClicked(input::Controls c) override;
			void ButtonReleased(input::Controls c) override;
		private:
			void init_all_windows(input::Keyboard* kb, game::CharacterStats *character_stats);
		};

	}
}