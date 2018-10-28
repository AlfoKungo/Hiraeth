#pragma once

#include "timer.h"

namespace hiraeth {

	class StaticTimer
	{
	public:
		static Timer timer;
		static void init() {}

	private:
		StaticTimer() {}
	};
}
