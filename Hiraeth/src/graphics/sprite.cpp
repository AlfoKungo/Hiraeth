#include "sprite.h"

namespace hiraeth {
	namespace graphics {

		Sprite::Sprite(float x, float y, float width, float height, const maths::vec4& color)
			: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), color)
		{

		}
		Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
			: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), maths::vec4(1.0f,1.0f,1.0f,1.0f))
		{
			m_Texture = texture;
		}
		Sprite::Sprite(float x, float y, float width, float height, Texture* texture, std::vector<maths::vec2> uv)
			: Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), uv, texture)
		{

		}
	}
}