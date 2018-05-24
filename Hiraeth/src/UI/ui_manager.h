#pragma once

#include <vector>
#include <algorithm>

#include "ui_window.h"
#include "ui_stats.h"
#include "ui_inventory.h"
#include "ui_skills.h"
#include "ui_quests.h"
#include "ui_equip.h"
#include "main_ui.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"
#include "game/character_stats.h"
#include "item/item_manager.h"

namespace hiraeth {
	namespace ui {

		class UiManager : public input::MouseEvent, public input::KeyboardEvent
		{
		private:
			graphics::Layer<UiWindow> m_Layer;
			std::vector<UiWindow*>& m_Windows;
			input::Keyboard* m_Kb;
			MainUi m_MainUi;
			UiInventory * m_UiInventory;
		public:
			UiManager(input::Keyboard* kb);
			void update();
			void draw() const;
			void set_keyboard(input::Keyboard* keyboard) { m_Kb = keyboard; }
			void leftButtonClicked(float mx, float my) override;
			void leftButtonReleased(float mx, float my) const override;
			void mouseMove(float pmx, float pmy, float mx, float my) const override;
			void ButtonClicked(input::Controls c) override;
			void ButtonReleased(input::Controls c) override;
			MainUi * getMainUi() { return &m_MainUi; }
			UiInventory * getUiInventory() { return m_UiInventory; }
		private:
			void init_all_windows(input::Keyboard* kb, game::CharacterStats *character_stats);
		};

	}
}