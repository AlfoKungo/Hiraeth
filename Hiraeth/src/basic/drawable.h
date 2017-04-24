#pragma once

namespace hiraeth {

		class Drawable
		{
		public:
			bool is_to_draw;
			virtual void draw() = 0;
		};
}
