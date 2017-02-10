#pragma once

namespace hiraeth {
	namespace basic {

		class Updatable
		{
		public:
			bool to_update;
			virtual void update() = 0;
		};
	}
}
