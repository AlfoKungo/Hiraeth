#pragma once

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexbuffer.h"
#include "graphics/buffers/vertexarray.h"

#include "graphics/texture.h"

#include "maths/maths.h"
#include "maprenderer.h"
#include "graphics/shader.h"
#include <cereal/access.hpp>

namespace hiraeth {
	namespace map {

		class MapRenderable
		{
		protected:
			maths::vec3 m_Position;
			maths::vec2 m_Size;
			unsigned int m_Color;
			std::vector<maths::vec2> m_UV;
		protected:
			MapRenderable()	{ }
			MapRenderable(maths::vec3 position, maths::vec2 size, std::vector<maths::vec2> uv)
				: m_Position(position), m_Size(size), m_UV(uv)
			{
				setColor(maths::vec4(1, 1, 1, 1));
			}
		public:
			virtual ~MapRenderable()
			{

			}

			virtual void submit(MapRenderer& renderer) const
			{
				renderer.submit(this);
			}

			void setColor(unsigned int color) { m_Color = color; }

			void setColor(const maths::vec4& color)
			{
				int r = (int)(color.x * 255.0f);
				int g = (int)(color.y * 255.0f);
				int b = (int)(color.z * 255.0f);
				int a = (int)(color.w * 255.0f);
				m_Color = a << 24 | b << 16 | g << 8 | r;
			}

			inline const maths::vec3& getPosition() const { return m_Position; }
			inline const maths::vec2& getSize() const { return m_Size; }
			inline const unsigned int getColor() const { return m_Color; }
			inline const std::vector<maths::vec2>& getUV() const
			{
				//if (m_UV.size() == 0)

				return m_UV;
			}
		};
	}
}
