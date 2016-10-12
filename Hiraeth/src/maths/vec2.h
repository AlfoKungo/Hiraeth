#pragma once

#include <iostream>
//#include <math.h>

namespace hiraeth {
	namespace maths {

		struct vec2
		{
			float x, y;
			
			vec2();
			vec2(const float& x, const float& y);

			vec2& add(const vec2& other);
			vec2& subtract(const vec2& other);
			vec2& multiply (const vec2& other);
			vec2& divide(const vec2& other);

			vec2& multiply (const float& other);
			vec2& divide(const float& other);

			friend vec2 operator+(vec2 left, const vec2& right);
			friend vec2 operator-(vec2 left, const vec2& right);
			friend vec2 operator*(vec2 left, const vec2& right);
			friend vec2 operator/(vec2 left, const vec2& right);

			friend vec2 operator*(vec2 left, const float& right);
			friend vec2 operator/(vec2 left, const float& right);

			bool operator==(const vec2& other);
			bool operator!=(const vec2& other);

			vec2& operator+=(const vec2& other);
			vec2& operator-=(const vec2& other);
			vec2& operator*=(const vec2& other);
			vec2& operator/=(const vec2& other);

			vec2& operator*=(const float& other);
			vec2& operator/=(const float& other);

			float distance_to(const vec2 other);

		template<class Archive>
		void serialize(Archive & archive) { archive(x,y); }

			friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);
		};

}
}