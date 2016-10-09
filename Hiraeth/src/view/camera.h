#pragma once

#include "maths\maths.h"
#include "graphics\window.h"

namespace hiraeth {
	namespace view {

		class Camera 
		{
		private:
			maths::mat4 m_Ortho;
			graphics::Window* m_Wnd;
		public:
			Camera(graphics::Window* wnd);
			~Camera();
			void update();

			maths::mat4 get_ortho() { return m_Ortho; }
		};
	}
}