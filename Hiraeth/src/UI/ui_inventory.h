#pragma once
#include "ui_window.h"
#include "graphics/texture_manager.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/label.h"

namespace hiraeth {
	namespace ui {
		class UiInventory : public UiWindow
		{
		private:

		public:
			UiInventory(maths::vec2 pos, input::Controls control_key);
			void fillGroup();
			void mouse_clicked() override {}
			void mouse_released() override {}
			void mouse_moved(float mx, float my) override {}
		};

	}
}