#pragma once
#include "ui_window.h"
#include "graphics/batchrenderer2d.h"
#include "maths/maths.h"
#include "graphics/texturemanager.h"
#include "graphics/sprite.h"
#include "graphics/spritedrenderable.h"
#include "graphics/label.h"
#include "utils/timer.h"
#include <string>
#include "game/stats.h"
#include "game/character_stats.h"

namespace hiraeth {
	namespace ui {


		class UiStats : public UiWindow
		{
		private:
			game::StatsStruct& m_StatsStruct;
			game::DetailsStruct& m_DetailsStruct;
		public:
			UiStats(maths::vec2 pos, input::Controls control_key, 
				game::CharacterStats *character_stats);
			void mouse_clicked() override {}
			void mouse_released() override {}
			void mouse_moved(float mx, float my) override {}
			void fillGroup();
		};

	}
}
