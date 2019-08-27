#pragma once


#include "graphics/renderable.h"
#include "maths/maths.h"
#include "graphics/window.h"
#include "utils/static_timer.h"

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
			static maths::mat4 m_Ortho;
			static const graphics::Renderable* m_Char;
			static float m_PositionTimer;
		public:
			static void init(graphics::Renderable* character);
			static void update();

			static maths::mat4 get_ortho()
			{ 
				return m_Ortho; 
			}
			static void setCharacter(graphics::Renderable* character);
			static maths::vec2 getCharacterPos();
			static maths::vec2 mouse_to_map_position(maths::vec2 mouse_pos);
		private:
			static void setNewPosition(maths::vec2 a, maths::vec2 b, maths::vec2 t);

			Camera() {}
		};
	}
}