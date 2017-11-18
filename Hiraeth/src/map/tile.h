#pragma once
#include "maths/maths.h"
#include "graphics/renderable.h"
#include "maprenderable.h"
#include "map_data.h"
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace hiraeth {
	namespace map {

		class Tile : public MapRenderable
		{
		private:
			float m_Scale;
			//maths::vec2 m_Uv_pos;
			//maths::vec2 m_Uv_size;
			//unsigned int m_Type;

		private:
			Tile(maths::vec3 pos, float scale, unsigned int type);
		public:
			explicit Tile(TileData s);
			~Tile();

			float get_scale() const { return m_Scale; }
			//maths::vec2 get_uv_pos() const { return m_Uv_pos; }
			//maths::vec2 get_uv_size() const { return m_Uv_size; }
			//unsigned int getType() const { return m_Type; }

			//maths::Rectangle get_rec() const
			//{
			//	return maths::Rectangle(m_Position, m_Uv_size * m_Scale);
			//}

		private:
			//std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
		};
	}
}