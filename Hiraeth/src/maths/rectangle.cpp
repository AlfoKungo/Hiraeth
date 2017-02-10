#include "rectangle.h"


namespace hiraeth {
	namespace maths {

		Rectangle::Rectangle()
			: position(vec2()), size(vec2())
		{
		}

		Rectangle::Rectangle(const vec2& position, const vec2& size)
			: position(position), size(size)
		{
		}

		Rectangle::Rectangle(float x, float y, float width, float height)
			: position(vec2(x, y)), size(vec2(width, height))
		{
		}

		bool Rectangle::Intersects(const Rectangle& other) const
		{
			//return (size > other.position && position < other.size) || (position > other.size && size < other.position);
			return (position + size > other.position && position < other.position + other.size) 
				|| (position > other.position + other.size && position + size < other.position);
			//std::cout << (size > other.position && position < other.size) << std::endl;
			//std::cout << (position > other.size && size < other.position) << std::endl;
			//std::cout << (position > other.position + other.size) << std::endl;
			//std::cout << (position + size < other.position) << std::endl;
			//return !((position > other.position + other.size) || (position + size < other.position));
		}

		Rectangle& Rectangle::Add(const vec2& other)
		{
			x += other.x;
			y += other.y;

			return *this;
		}

		Rectangle operator+(Rectangle left, const vec2& right)
		{
			return left.Add(right);
		}

		vec2 Rectangle::Distance(const Rectangle& other) const
		{
			float x_distance = 0, y_distance = 0;
			if (position.x > other.position.x + other.size.x)
				x_distance = abs(position.x - other.position.x - other.size.x);
			else
				x_distance = abs(other.position.x - position.x - size.x);
			if (position.y > other.position.y + other.size.y)
				y_distance = abs(position.y - other.position.y - other.size.y);
			else
				y_distance = abs(other.position.y - position.y - size.y);
			return vec2(x_distance, y_distance);
		}

		bool Rectangle::Contains(const vec2& point) const
		{
			return point > GetMinimumBound() && point < GetMaximumBound();
		}

		bool Rectangle::Contains(const vec3& point) const
		{
			return Contains(vec2(point));
		}

		bool Rectangle::operator==(const Rectangle& other) const
		{
			return position == other.position && size == other.size;
		}

		bool Rectangle::operator!=(const Rectangle& other) const
		{
			return !(*this == other);
		}

		bool Rectangle::operator<(const Rectangle& other) const
		{
			return size < other.size;
		}

		bool Rectangle::operator>(const Rectangle& other) const
		{
			return size > other.size;
		}

	}
}