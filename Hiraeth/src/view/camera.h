#pragma once

#include "graphics\renderable2d.h"
#include "maths\maths.h"
#include "graphics\window.h"
#include "utils\timer.h"

namespace hiraeth {
	namespace view {

#define CAMERA_TIME_BETWEEN_MOVEMENT 0.001f
#define CAMERA_VP_SIZE_X 1600
#define CAMERA_VP_SIZE_X_HALF 800
#define CAMERA_VP_SIZE_Y 900
#define CAMERA_VP_SIZE_Y_HALF 450
#define CAMERA_X_LERP_VALUE 0.040f
#define CAMERA_Y_LERP_VALUE 0.085f

		class Camera 
		{
		private:
			maths::mat4 m_Ortho;
			graphics::Window* m_Wnd;
			graphics::Renderable2D* m_Char;
			float m_PositionTimer = 0;
			Timer* m_Time;
		public:
			Camera(graphics::Window* wnd, Timer* time);
			~Camera();
			void update();

			maths::mat4 get_ortho() { return m_Ortho; }
			void setCharacter(graphics::Renderable2D* character);
		private:
			void setNewPosition(maths::vec2 a, maths::vec2 b, maths::vec2 t);
		};
	}
}