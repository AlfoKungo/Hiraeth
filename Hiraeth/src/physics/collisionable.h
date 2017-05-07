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
			virtual ~Collisionable() {}
		protected:
			int analyzeCollisionX(const maths::Rectangle& char_rec, const maths::vec2& char_speed) const;
			int analyzeCollisionY(const maths::Rectangle& char_rec, const maths::vec2& char_speed) const;
			void set_foothold(int foothold);
			bool check_if_still_on_foothold() const;
			maths::vec2 force_by_vertical_foothold(const maths::vec2& force, int footholdIndex) const;
			maths::vec2 set_y_by_foothold(const maths::vec2& force)const ;
		private:
		};
	}
}