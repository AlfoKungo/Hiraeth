#pragma once
#include "maths/rectangle.h"

namespace hiraeth {
	namespace physics {
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

		public:
			FootHold(maths::vec2 p1, maths::vec2 p2, maths::vec2 direction);
			bool Intersects(maths::Rectangle rec);
			bool CohenSutherlandLineClipAndDraw(maths::Rectangle rec);
			bool LinesIntersect(maths::vec2 lp1, maths::vec2 lp2);
			inline bool is_solid() { return (p1.x - p2.x != 0); }
		private:
			OutCode ComputeOutCode(double x, double y, maths::Rectangle rec);
		};

		class HorizontalFootHold : public FootHold
		{
		public:
			HorizontalFootHold(maths::vec2 p1, float length)
				: FootHold(p1 - maths::vec2(22.5f, -3), maths::vec2(p1.x + length + 22.5f, p1.y + 3), maths::vec2(0, 1))
			{}
			HorizontalFootHold(maths::vec2 p1, maths::vec2 p2)
				: FootHold(p1 - maths::vec2(22.5f, -3), p2 + maths::vec2(22.5f, 3), maths::vec2(0, 1))
			{}
		};

		class VerticalFootHold : public FootHold
		{
		public:
			VerticalFootHold(maths::vec2 p1, float length)
				: FootHold(p1, maths::vec2(p1.x, p1.y + length), maths::vec2(1, 0))
			{}
			VerticalFootHold(maths::vec2 p1, maths::vec2 p2)
				: FootHold(p1, p2, maths::vec2(1, 0))
			{}
		};
	}
}
