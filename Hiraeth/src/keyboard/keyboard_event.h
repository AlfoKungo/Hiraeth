#pragma once

namespace hiraeth {
	namespace input {
		enum Controls;
		class KeyboardEvent
		{
		public:
			virtual void ButtonClicked(Controls control) = 0;
			virtual void ButtonReleased(Controls control) = 0;
			virtual ~KeyboardEvent() {}
		};
	}
}