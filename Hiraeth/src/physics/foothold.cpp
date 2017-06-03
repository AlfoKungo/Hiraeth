#include "foothold.h"

namespace hiraeth {
	namespace physics {


		FootHold::FootHold(maths::vec2 p1, maths::vec2 p2, maths::vec2 direction)
			: p1(p1), p2(p2), m_Direction(direction)
		{

		}

		bool FootHold::Intersects(maths::Rectangle rec)
		{
			return CohenSutherlandLineClipAndDraw(rec);
		}

		bool FootHold::LinesIntersect(maths::vec2 lp1, maths::vec2 lp2)
		{
			return (((lp1.x - p1.x)*(p2.y - p1.y) - (lp1.y - p1.y)*(p2.x - p1.x)) * ((lp2.x - p1.x)*(p2.y - p1.y) - (lp2.y - p1.y)*(p2.x - p1.x)) < 0
				&&
				((p1.x - lp1.x)*(lp2.y - lp1.y) - (p1.y - lp1.y)*(lp2.x - lp1.x)) * ((p2.x - lp1.x)*(lp2.y - lp1.y) - (p2.y - lp1.y)*(lp2.x - lp1.x)) < 0);
		}

		OutCode FootHold::ComputeOutCode(double x, double y, maths::Rectangle rec)
		{
			OutCode code;

			code = INSIDE;          // initialised as being inside of [[clip window]]

			if (x < rec.x)           // to the left of clip window
				code |= LEFT;
			else if (x > rec.x + rec.width)      // to the right of clip window
				code |= RIGHT;
			if (y < rec.y)           // below the clip window
				code |= BOTTOM;
			else if (y > rec.y + rec.height)      // above the clip window
				code |= TOP;

			return code;
		}

		// Cohen–Sutherland clipping algorithm clips a line from
		// P0 = (x0, y0) to P1 = (x1, y1) against a rectangle with 
		// diagonal from (xmin, ymin) to (xmax, ymax).
		bool FootHold::CohenSutherlandLineClipAndDraw(maths::Rectangle rec)
		{
			double x0 = p1.x, y0 = p1.y, x1 = p2.x, y1 = p2.y;
			double xmin = rec.x, ymin = rec.y, xmax = rec.x + rec.width, ymax = rec.y + rec.height;

			// compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
			OutCode outcode0 = ComputeOutCode(x0, y0, rec);
			OutCode outcode1 = ComputeOutCode(x1, y1, rec);
			bool accept = false;

			while (true) 
			{
				if ((!outcode0) | (!outcode1))  // Bitwise OR is 0. Trivially accept and get out of loop
					return true;
				if (outcode0 & outcode1)  // Bitwise AND is not 0. Trivially reject and get out of loop
					return false;
				// failed both tests, so calculate the line segment to clip
				// from an outside point to an intersection with clip edge
				double x, y;

				// At least one endpoint is outside the clip rectangle; pick it.
				OutCode outcodeOut = outcode0 ? outcode0 : outcode1;

				// Now find the intersection point;
				// use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
				if (outcodeOut & TOP) {           // point is above the clip rectangle
					x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
					y = ymax;
				}
				else if (outcodeOut & BOTTOM) { // point is below the clip rectangle
					x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
					y = ymin;
				}
				else if (outcodeOut & RIGHT) {  // point is to the right of clip rectangle
					y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
					x = xmax;
				}
				else if (outcodeOut & LEFT) {   // point is to the left of clip rectangle
					y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
					x = xmin;
				}

				// Now we move outside point to intersection point to clip
				// and get ready for next pass.
				if (outcodeOut == outcode0) {
					x0 = x;
					y0 = y;
					outcode0 = ComputeOutCode(x0, y0, rec);
				}
				else {
					x1 = x;
					y1 = y;
					outcode1 = ComputeOutCode(x1, y1, rec);
				}
			}
		}
	}
}
