#pragma once
#include "maths/rectangle.h"
#include "srl/map_data.h"
#include <cereal/access.hpp>

namespace hiraeth {
	namespace physics {

#define NO_FOOTHOLD -1
		typedef int FootHoldIndex;
		struct CollisionStruct
		{
			int x, y;
		};
		typedef int OutCode;

		const int INSIDE = 0; // 0000
		const int LEFT = 1;   // 0001
		const int RIGHT = 2;  // 0010
		const int BOTTOM = 4; // 0100
		const int TOP = 8;    // 1000

							  // Compute the bit code for a point (x, y) using the clip rectangle
							  // bounded diagonally by (xmin, ymin), and (xmax, ymax)

							  // ASSUME THAT xmax, xmin, ymax and ymin are global constants.
		struct FootHold
		{
		private:
		public:
			maths::vec2 p1, p2;
			maths::vec2 m_Direction;

		private:
			FootHold(maths::vec2 p1, maths::vec2 p2, maths::vec2 direction);
		public:
			explicit FootHold(SRL::FootHoldData fh_data)
				: FootHold(fh_data.p1, fh_data.p2, fh_data.direction) {}
			~FootHold() {}
			bool isOnFoothold(maths::vec2 pt) const;
			bool Intersects(maths::Rectangle rec) const;
			bool CohenSutherlandLineClipAndDraw(maths::Rectangle rec) const;
			bool LinesIntersect(maths::vec2 lp1, maths::vec2 lp2) const;
			inline bool is_solid() const { return (p1.x - p2.x != 0); }
			float getXStart() const { return p1.x; }
			float getXEnd() const { return p2.x; }
		private:
			OutCode ComputeOutCode(double x, double y, maths::Rectangle rec) const;
		};

	}
}
