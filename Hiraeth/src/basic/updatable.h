#pragma once

namespace hiraeth {

		class Updatable
		{
		public:
			bool is_to_update;
			virtual void update() = 0;
		};
}
