#pragma once
#include "ui_window.h"
#include "graphics\layers\layer.h"
#include "graphics\batchrenderer2d.h"
#include "maths\maths.h"
#include "graphics\texturemanager.h"
#include "graphics\sprite.h"
#include "graphics\spritedrenderable.h"
#include "graphics\label.h"
#include "utils\timer.h"
#include <string>

namespace hiraeth {
	namespace ui {

		struct StatsStruct
		{
			std::string Name, Job;
			unsigned int Level;
			unsigned int Exp, HonorLvl, HonorExp;
			std::string Guild;
			unsigned int Hp, Mp;
			unsigned int Fame;
			unsigned int AbilityPoints;
			unsigned int Str, Int, Luk, Dex;
		};
		class Stats : public UiWindow
		{
		private:
			StatsStruct m_StatsStruct;
			graphics::Layer m_Layer;
		public:
			Stats(maths::vec2 pos, Timer* time);
			void draw();
			void update();
			void mouse_clicked() {}
			void mouse_released() {}
			void mouse_moved(float mx, float my) {}
		private:
		};

	}
}
