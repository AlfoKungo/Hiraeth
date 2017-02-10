#pragma once
#include "keyboard\keyboard_event.h"

namespace hiraeth {
	namespace game {

		class Stats : input::KeyboardEvent
		{
		private:
			unsigned int Str, Int, Luk, Dex;
			unsigned int Accuracy, Avoidablity;
			unsigned int Hp, Mp;
			unsigned int Fame;
		public:
			Stats();
			void ButtonClicked() override;
		private:
		};

	}
}