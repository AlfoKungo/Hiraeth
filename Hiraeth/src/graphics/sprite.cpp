#include "sprite.h"

namespace hiraeth {
	namespace graphics {

		Sprite::Sprite(maths::vec2 pos, float width, float height, const unsigned int color)
			: Renderable(maths::vec3{pos}, maths::vec2(width, height), color)
		{

		}
		Sprite::Sprite(maths::vec2 pos, Texture* texture)
			: Renderable(maths::vec3{pos}, texture->getSize(), 0xffffffff)
		{
			m_Texture = texture;
		}
		Sprite::Sprite(maths::vec2 pos, maths::vec2 org, Texture* texture)
			: Renderable(maths::vec3{pos}, texture->getSize(), 0xffffffff, org)
		{
			m_Texture = texture;
		}

		Sprite::Sprite(maths::vec2 pos)
			: Renderable(maths::vec3{pos}, maths::vec2{0,0}, 0xffffffff)
		{
		}

		Sprite::Sprite(maths::vec2 pos, float width, float height, Texture* texture, std::vector<maths::vec2> uv)
			: Renderable(maths::vec3{pos}, maths::vec2(width, height), uv, texture)
		{

		}

		void Sprite::setTexture(Texture* texture)
		{
			m_Texture = texture;
			m_Bounds.size = texture->getSize();
		}
	}
}