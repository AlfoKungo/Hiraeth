#pragma once

namespace hiraeth {
	namespace game {

		class StatsUpdateEvent
		{
		public:
			virtual void StatsUpdated() = 0;
			virtual ~StatsUpdateEvent() {}
		};
	}
}
