#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "vec3.h"
#include "../../Dependencies/Cereal/include/cereal/cereal.hpp"

namespace maths {

	struct vec2
	{
		float x, y;

		vec2();
		vec2(float scalar);
		vec2(float x, float y);
		vec2(const vec3& vector);

		vec2& Add(const vec2& other);
		vec2& Subtract(const vec2& other);
		vec2& Multiply(const vec2& other);
		vec2& Divide(const vec2& other);

		vec2& Add(float value);
		vec2& Subtract(float value);
		vec2& Multiply(float value);
		vec2& Divide(float value);

		friend vec2 operator+(vec2 left, const vec2& right);
		friend vec2 operator-(vec2 left, const vec2& right);
		friend vec2 operator*(vec2 left, const vec2& right);
		friend vec2 operator/(vec2 left, const vec2& right);

		friend vec2 operator+(vec2 left, float value);
		friend vec2 operator-(vec2 left, float value);
		friend vec2 operator*(vec2 left, float value);
		friend vec2 operator/(vec2 left, float value);

		bool operator==(const vec2& other) const;
		bool operator!=(const vec2& other) const;

		vec2& operator+=(const vec2& other);
		vec2& operator-=(const vec2& other);
		vec2& operator*=(const vec2& other);
		vec2& operator/=(const vec2& other);

		vec2& operator+=(float value);
		vec2& operator-=(float value);
		vec2& operator*=(float value);
		vec2& operator/=(float value);

		bool operator<(const vec2& other) const;
		bool operator<=(const vec2& other) const;
		bool operator>(const vec2& other) const;
		bool operator>=(const vec2& other) const;

		float Magnitude() const;
		float MagnitudeU() const;
		vec2 Normalise() const;
		float Distance(const vec2& other) const;
		float Dot(const vec2& other) const;
		//vec2 lerp(const vec2& other, float t) const;

		std::string ToString() const;

		friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);

		template<class Archive>
		void serialize(Archive & archive) { archive(CEREAL_NVP(x), CEREAL_NVP(y)); }

		friend std::ostream& operator<<(std::ostream& stream, const vec2& vector);
	};

}
