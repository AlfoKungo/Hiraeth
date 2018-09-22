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

			Collisionable(maths::Rectangle& box, map::MapLayer* map_layer);
			Collisionable(maths::Rectangle& empty_box, map::MapLayer* map_layer,
				unsigned int foothold);
			virtual ~Collisionable() = default;
		protected:
			void analyze_collision();
		private:
			int analyze_collision_x(const maths::vec2& char_speed) const;
			int analyze_collision_y(const maths::vec2& char_speed) const;
			bool check_if_still_on_foothold() const;
			virtual void move(const maths::vec2& step) = 0;
			maths::vec2 force_by_vertical_foothold(const maths::vec2& force, int footholdIndex) const;
			maths::vec2 set_y_by_foothold(const maths::vec2& force)const ;
		};
	}
}