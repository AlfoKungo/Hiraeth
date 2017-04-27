#pragma once

#include "graphics\window.h"
#include "map\maplayer.h"

namespace hiraeth {
	namespace physics {

		class Collisionable
		{

#define NO_FOOTHOLD -1

		protected:
			int m_Foothold, m_LastFoothold;
			map::MapLayer* m_MapLayer;
			maths::Rectangle& m_Box;

			Collisionable(maths::Rectangle& box, map::MapLayer* m_MapLayer);
		protected:
			physics::CollisionStruct analyze_collision(maths::Rectangle char_rec, maths::vec2 char_speed);
			void set_foothold(int foothold);
			bool check_if_still_on_foothold();
			maths::vec2 force_by_vertical_foothold(const maths::vec2& force, int footholdIndex);
			maths::vec2 set_y_by_foothold(const maths::vec2& force);
		private:
		};
	}
}