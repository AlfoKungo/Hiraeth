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
#include "ui_keyconfig.h"
#include "ui_party.h"
#include "ui_trade.h"
#include "keyboard/keyboard.h"
#include "keyboard/keyboard_event.h"
#include "game/character_stats.h"
#include "item/item_manager.h"
#include "UI/ui_shop.h"

namespace hiraeth {
	namespace ui {

		class UiManager : public input::MouseEvent, public input::KeyboardEvent
		{
		private:
			graphics::Layer<UiWindow> m_Layer;
			std::vector<UiWindow*>& m_Windows; // change to std::map
			input::Keyboard* m_Kb;
			MainUi m_MainUi;
			UiInventory * m_UiInventory;
			UiStats * m_UiStats;
			UiSkills * m_UiSkills;
			UiEquip * m_UiEquip;
			UiQuests * m_UiQuests;
			UiKeyConfig * m_UiKeyConfig;
			UiParty* m_UiParty;
			UiTrade * m_UiTrade;
			UiShop * m_UiShop;
			struct MouseAction { bool is_occupied; UiWindow * rel_win; }; // not yet implemented - supposed to be used for data transfer between windows.
		public:
			UiManager(input::Keyboard* kb);
			void update();
			void draw() const;
			//void set_keyboard(input::Keyboard* keyboard) { m_Kb = keyboard; }
			bool leftButtonClicked(float mx, float my) override;
			bool leftButtonReleased(float mx, float my)  override;
			bool rightButtonClicked(float mx, float my) override;
			bool mouseMove(float pmx, float pmy, float mx, float my)  override;
			bool is_window_contains(maths::vec2 mouse_pos) const override;
			void ButtonClicked(input::Key key) override;
			void ButtonReleased(input::Key key) override;
			MainUi * getMainUi() { return &m_MainUi; }
			UiInventory * getUiInventory() { return m_UiInventory; }
			UiStats * getUiStats() { return m_UiStats; }
			UiSkills * getUiSkills() { return m_UiSkills; }
			UiEquip * getUiEquip() { return m_UiEquip; }
			UiQuests * getUiQuests() { return m_UiQuests; }
			UiParty * getUiParty() { return m_UiParty; }
			UiTrade * getUiTrade() { return m_UiTrade; }
			UiShop * getUiShop() { return m_UiShop; }
		private:
			void init_all_windows(input::Keyboard* kb, game::CharacterStats *character_stats);
		};

	}
}