#pragma once
#include "ui_window.h"
#include "ui_basic/ui_button.h"
#include "ui_basic/ui_button_flashing.h"
#include "ui_basic/ui_button_side_menu.h"
#include "graphics/renderer.h"
#include "maths/maths.h"
#include "graphics/texture_manager.h"
#include "graphics/sprite.h"
#include "graphics/sprited_renderable.h"
#include "graphics/label.h"
#include "utils/timer.h"
#include <string>
#include "game/stats.h"
#include "game/character_stats.h"

#include <iostream>

namespace hiraeth {
	namespace ui {


		class UiEquip : public UiWindow
		{
		private:
		public:
			UiEquip(maths::vec2 pos, input::Controls control_key);
			void mouse_left_clicked(maths::vec2 mousePos);
			void mouse_left_released(maths::vec2 mousePos) override {}
			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override {}
			void fillGroup();
			void StatsUpdated();
		};

	}
}
