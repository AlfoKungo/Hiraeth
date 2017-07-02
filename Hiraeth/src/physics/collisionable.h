#pragma once

#include "graphics/window.h"
#include "map/maplayer.h"

namespace hiraeth {
	namespace physics {

		class Collisionable
		{


		protected:
			int m_Foothold;
			int m_LastFoothold;
			map::MapLayer* m_MapLayer;
			const maths::Rectangle& m_Box;
			maths::vec2 m_Force;

			Collisionable(maths::Rectangle& box, map::MapLayer* m_MapLayer);
			virtual ~Collisionable() {}
		protected:
			void analyzeCollision();
		private:
			int analyzeCollisionX(const maths::vec2& char_speed) const;
			int analyzeCollisionY(const maths::vec2& char_speed) const;
			bool check_if_still_on_foothold() const;
			virtual void move(const maths::vec2& step) = 0;
			maths::vec2 force_by_vertical_foothold(const maths::vec2& force, int footholdIndex) const;
			maths::vec2 set_y_by_foothold(const maths::vec2& force)const ;
		};
	}
}