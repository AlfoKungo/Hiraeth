#pragma once

namespace hiraeth {
	namespace basic {

		class Drawable
		{
		public:
			bool to_draw;
			virtual void draw() = 0;
		};
	}
}
