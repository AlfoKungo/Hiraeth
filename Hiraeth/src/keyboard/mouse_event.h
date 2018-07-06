#pragma once
#include "maths/maths.h"

namespace hiraeth {
	namespace input {
		class MouseEvent
		{
		public:
			virtual void leftButtonClicked(float mx, float my) = 0;
			virtual void rightButtonClicked(float mx, float my) = 0;
			virtual void leftButtonReleased(float mx, float my) const = 0;
			virtual void mouseMove(float pmx, float pmy, float mx, float my) const = 0;
			virtual bool is_window_contains(maths::vec2 mouse_pos) const = 0;
			virtual ~MouseEvent() {}


		};
	}
}
