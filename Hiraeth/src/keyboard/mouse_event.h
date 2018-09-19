#pragma once
#include "maths/maths.h"

namespace hiraeth {
	namespace input {
		class MouseEvent
		{
		public:
			virtual bool leftButtonClicked(float mx, float my) = 0;
			virtual bool rightButtonClicked(float mx, float my) = 0;
			virtual bool leftButtonReleased(float mx, float my) const = 0;
			virtual bool mouseMove(float pmx, float pmy, float mx, float my) const = 0;
			virtual bool is_window_contains(maths::vec2 mouse_pos) const = 0;
			virtual ~MouseEvent() {}


		};
	}
}
