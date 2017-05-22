#pragma once

namespace hiraeth {

		class Updatable
		{
		public:
			bool is_to_update = false;
			virtual void update() = 0;
			virtual ~Updatable() {}
		};
}
