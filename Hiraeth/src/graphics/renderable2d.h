#pragma once

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

#include "renderer2d.h"
#include "texture.h"
#include "basic/drawable.h"
#include "basic/updatable.h"

#include "../maths/maths.h"
#include "shader.h"

namespace hiraeth {
	namespace graphics {

		struct VertexData
		{
			maths::vec3 vertex;
			maths::vec2 uv;
			float tid;
			unsigned int color;
		};

		class Renderable2D : public Drawable, public Updatable
		{
		protected:
			maths::Rectangle m_Bounds;
			unsigned int m_Color;
			std::vector<maths::vec2> m_UV;
			Texture* m_Texture;
		protected:
			Renderable2D() : m_Texture(nullptr) { setUVDefaults(); }
		public:
			Renderable2D(maths::vec3 position, maths::vec2 size, unsigned int color)
				: m_Bounds(position, size), m_Color(color)
			{
				setColor(color);
				setUVDefaults();
			}
			Renderable2D(maths::vec3 position, maths::vec2 size, std::vector<maths::vec2> uv, Texture* texture)
				: m_Bounds(position, size), m_Color(0xffffffff)
			{
				m_Texture = texture;
				for (std::vector<maths::vec2>::const_iterator it = uv.begin(); it != uv.end(); it++)
					m_UV.push_back((*it) / maths::vec2((*m_Texture).getWidth(), (*m_Texture).getHeight()));
			}

			virtual ~Renderable2D()
			{
			}

			//virtual void submit(Renderer2D* renderer) const
			//{
			//	renderer->submit(this);
			//}

			inline void setColor(unsigned int color) { m_Color = color; }

			// gets
			inline const maths::vec2& getPosition() const { return m_Bounds.position; }
			inline const maths::vec2& getSize() const { return m_Bounds.size; }
			inline const maths::Rectangle& getBounds() const { return m_Bounds; }
			inline maths::Rectangle& getBounds() { return m_Bounds; }
			inline Texture* getTexture() const { return m_Texture; }
			inline const unsigned int getColor() const { return m_Color; }
			inline const std::vector<maths::vec2>& getUV() const { return m_UV; }
			inline const GLuint getTID() const { return m_Texture == nullptr ? 0 : m_Texture->getID(); }

			// sets
			inline void setPosition(const maths::vec2& position) { m_Bounds.position = position; }
			inline void setSize(const maths::vec2& size) { m_Bounds.size = size; }
			inline void move(const maths::vec2& step) { m_Bounds.position += step; }

			virtual void update() {}
			virtual void draw(Renderer2D* renderer) const
			{ 
				renderer->submit(this); 
			}
		private:
			void setUVDefaults()
			{
				m_UV.push_back(maths::vec2(0, 0));
				m_UV.push_back(maths::vec2(0, 1));
				m_UV.push_back(maths::vec2(1, 1));
				m_UV.push_back(maths::vec2(1, 0));
			}
		};
	}
}