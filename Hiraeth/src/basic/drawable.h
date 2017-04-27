#pragma once

namespace hiraeth {

		class Drawable
		{
		public:
			bool is_to_draw = false;
			virtual void draw() = 0;
		};
}
