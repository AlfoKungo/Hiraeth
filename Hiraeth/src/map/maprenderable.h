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
			unsigned int m_Color;
			unsigned int m_Type;
		protected:
			MapRenderable()	{ }
			MapRenderable(maths::vec3 position, unsigned int type)
				: m_Position(position), m_Type(type)
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
				int r = int(color.x * 255.0f);
				int g = int(color.y * 255.0f);
				int b = int(color.z * 255.0f);
				int a = int(color.w * 255.0f);
				m_Color = a << 24 | b << 16 | g << 8 | r;
			}

			inline const maths::vec3& getPosition() const { return m_Position; }
			inline unsigned int getColor() const { return m_Color; }
			unsigned int getType() const { return m_Type; }
		};
	}
}
