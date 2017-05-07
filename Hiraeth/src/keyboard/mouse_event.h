#pragma once

namespace hiraeth {
	namespace input {
		class MouseEvent
		{
		public:
			virtual void leftButtonClicked(float mx, float my) = 0;
			virtual void leftButtonReleased(float mx, float my) const = 0;
			virtual void mouseMove(float pmx, float pmy, float mx, float my) const = 0;
			virtual ~MouseEvent() {}


		};
	}
}
