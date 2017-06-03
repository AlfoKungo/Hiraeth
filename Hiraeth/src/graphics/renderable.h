#pragma once

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

#include "texture.h"
#include "basic/drawable.h"
#include "basic/updatable.h"

#include "../maths/maths.h"
#include "shader.h"

namespace hiraeth {
	namespace graphics {


		class Renderable : public Drawable, public Updatable
		{
		protected:
			maths::Rectangle m_Bounds;
			unsigned int m_Color;
			std::vector<maths::vec2> m_UV;
			maths::vec2 m_Org;
			Texture* m_Texture;
		protected:
			Renderable() : m_Color(0xffffffff), m_Texture(nullptr) { setUVDefaults(); }
		public:
			Renderable(maths::vec3 position, maths::vec2 size, unsigned int color, maths::vec2 origin = maths::vec2(0))
				: m_Bounds(position, size), m_Color(color), m_Texture(nullptr), m_Org(origin)
			{
				setColor(color);
				setUVDefaults();
			}
			Renderable(maths::vec3 position, maths::vec2 size, std::vector<maths::vec2> uv, Texture* texture, maths::vec2 origin = maths::vec2(0))
				: m_Bounds(position, size), m_Color(0xffffffff), m_Org(origin)
			{
				m_Texture = texture;
				m_UV = uv;
				//for (std::vector<maths::vec2>::const_iterator it = uv.begin(); it != uv.end(); it++)
				//	m_UV.push_back((*it) / maths::vec2((*m_Texture).getWidth(), (*m_Texture).getHeight()));
			}

			virtual ~Renderable()
			{
			}


			// gets
			inline const maths::vec2& getPosition() const { return m_Bounds.position; }
			inline const maths::vec2& getSize() const { return m_Bounds.size; }
			inline const maths::Rectangle& getBounds() const { return m_Bounds; }
			inline maths::Rectangle& getBounds() { return m_Bounds; }
			inline Texture* getTexture() const { return m_Texture; }
			inline const unsigned int getColor() const { return m_Color; }
			inline const std::vector<maths::vec2>& getUV() const { return m_UV; }
			inline const maths::vec2& getOrigin() const { return m_Org; }
			//inline GLuint getTID() const { return m_Texture == nullptr ? 0 : m_Texture->getID(); }
			GLuint getTID() const
			{
				if (m_Texture == nullptr)
					return 0;
				return m_Texture->getID();
			}

			// sets
			inline void setColor(unsigned int color) { m_Color = color; }
			inline void setPosition(const maths::vec2& position) { m_Bounds.position = position; }
			inline void setSize(const maths::vec2& size) { m_Bounds.size = size; }

			virtual void update() override {}
			virtual void draw(Renderer* renderer) const override
			{ 
				renderer->submit(this); 
			}
			virtual void draw(Renderer* renderer, unsigned int color)
			{ 
				renderer->submit(this, color); 
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