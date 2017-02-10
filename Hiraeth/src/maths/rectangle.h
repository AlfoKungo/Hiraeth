#pragma once


#include "vec2.h"
#include "vec3.h"

namespace hiraeth {
	namespace maths {


		/// Rectangle
		/// Note that position is stored as a center point, and size is
		/// half-width and half-height extents.
		struct Rectangle
		{
			union
			{
				vec2 position;
				struct
				{
					float x;
					float y;
				};
			};
			union
			{
				vec2 size;
				struct
				{
					float width;
					float height;
				};
			};

			Rectangle();
			Rectangle(const vec2& position, const vec2& size);
			Rectangle(float x, float y, float width, float height);

			Rectangle& Add(const vec2& value);
			friend Rectangle operator+(Rectangle left, const vec2& right);

			bool Intersects(const Rectangle& other) const;
			bool Contains(const vec2& point) const;
			bool Contains(const vec3& point) const;
			vec2 Distance(const Rectangle& other) const;


			inline vec2 GetMinimumBound() const { return position; }
			inline vec2 GetMaximumBound() const { return position + size; }

			bool operator==(const Rectangle& other) const;
			bool operator!=(const Rectangle& other) const;

			bool operator<(const Rectangle& other) const;
			bool operator>(const Rectangle& other) const;

			friend std::ostream& operator<<(std::ostream& stream, const Rectangle& Rectangle);
		};

	}
}