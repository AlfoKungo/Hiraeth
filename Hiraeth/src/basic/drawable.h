#pragma once

namespace hiraeth {

		class Drawable
		{
		public:
			bool is_to_draw = true;
			virtual void draw() = 0;
			virtual ~Drawable() {}
		};
}
