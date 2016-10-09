#pragma once
#include "maths\maths.h"
#include "graphics\renderable2d.h"
#include "maprenderable.h"
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
			maths::vec2 m_Uv_pos;
			maths::vec2 m_Uv_size;
			unsigned int m_Type;

		public:
			Tile(maths::vec3 pos, float scale, maths::vec2 uv_pos, maths::vec2 uv_size, unsigned int type);
			Tile(maths::vec2 pos, float scale, maths::vec2 uv_pos, maths::vec2 uv_size, unsigned int type);

			float get_scale() { return m_Scale; }
			maths::vec2 get_uv_pos() { return m_Uv_pos; }
			maths::vec2 get_uv_size() { return m_Uv_size; }
			float get_type() { return m_Type; }

		template<typename Archive>
		static void load_and_construct(Archive &ar, cereal::construct<Tile> &construct) 
		{
			maths::vec3 position;
			float scale;
			maths::vec2 uv_pos;
			maths::vec2 uv_size;
			unsigned int type;
			ar(position, scale, uv_pos, uv_size, type);
			construct(position, scale, uv_pos, uv_size, type);
		}
		template<class Archive>
		void serialize(Archive & ar) 
		{ 
			ar(m_Position, m_Scale, m_Uv_pos, m_Uv_size, m_Type); 
		}

		private:
			Tile::Tile();
			friend class cereal::access;

			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
		};
	}
}