#pragma once

namespace hiraeth {

		class Updatable
		{
		public:
			bool is_to_update = true;
			virtual void update() = 0;
			virtual ~Updatable() {}
		};
}
