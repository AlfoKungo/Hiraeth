#pragma once


#include "graphics/renderable.h"
#include "maths/maths.h"
#include "graphics/window.h"
#include "utils/static_timer.h"

//#define EDITOR
namespace hiraeth {
	namespace view {

constexpr auto CAMERA_TIME_BETWEEN_MOVEMENT = 0.001f;
constexpr auto CAMERA_VP_SIZE_X = 1600;
constexpr auto CAMERA_VP_SIZE_X_HALF = 800;
constexpr auto CAMERA_VP_SIZE_Y = 900;
constexpr auto CAMERA_VP_SIZE_Y_HALF = 450;
#ifndef EDITOR
constexpr auto CAMERA_X_LERP_VALUE = 0.040f;
constexpr auto CAMERA_Y_LERP_VALUE = 0.085f;
#else
constexpr auto CAMERA_X_LERP_VALUE = 1.0f;
constexpr auto CAMERA_Y_LERP_VALUE = 1.0f;
#endif

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
			static maths::vec2 mouse_to_screen_position(maths::vec2 mouse_pos);
		private:
			static void setNewPosition(maths::vec2 char_pos, maths::vec2 cam_pos, maths::vec2 lerp);

			Camera() {}
		};
	}
}