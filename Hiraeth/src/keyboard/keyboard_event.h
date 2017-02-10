#pragma once

namespace hiraeth {
	namespace input {

		class KeyboardEvent
		{
		public:
			virtual void ButtonClicked() = 0;
			virtual void ButtonReleased() = 0;
		};
	}
}