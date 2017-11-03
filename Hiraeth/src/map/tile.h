#pragma once
#include "maths/maths.h"
#include "graphics/renderable.h"
#include "maprenderable.h"
#include <cereal/access.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>

namespace hiraeth {
	namespace map {

		class Tile : public MapRenderable
		{
		public:
			struct TileData
			{
				maths::vec2 position;
				float scale;
				maths::vec2 uv_pos;
				maths::vec2 uv_size;
				unsigned int type;
				template<class Archive>
				void serialize(Archive & ar)
				{
					ar(position, scale, uv_pos, uv_size, type);
				}
			};
		private:
			float m_Scale;
			maths::vec2 m_Uv_pos;
			maths::vec2 m_Uv_size;
			unsigned int m_Type;

		public:
			Tile(maths::vec3 pos, float scale, maths::vec2 uv_pos, maths::vec2 uv_size, unsigned int type);
			Tile(maths::vec2 pos, float scale, maths::vec2 uv_pos, maths::vec2 uv_size, unsigned int type);
			explicit Tile(TileData s);
			~Tile();

			float get_scale() const { return m_Scale; }
			maths::vec2 get_uv_pos() const { return m_Uv_pos; }
			maths::vec2 get_uv_size() const { return m_Uv_size; }
			unsigned int get_type() const { return m_Type; }

			maths::Rectangle get_rec() const
			{
				return maths::Rectangle(m_Position, m_Uv_size * m_Scale);
			}

		private:

			friend class cereal::access;
			template <class Archive>
			static void load_and_construct(Archive & ar, cereal::construct<Tile> & construct)
			{
				maths::vec3 pos;
				float scale;
				maths::vec2 uv_pos;
				maths::vec2 uv_size;
				unsigned int type;
				ar(pos, scale, uv_pos, uv_size, type);
				construct(pos, scale, uv_pos, uv_size, type); // calls MyType( x )
			}
			template<class Archive>
			void serialize(Archive & ar)
			{
				ar(m_Position, m_Scale, m_Uv_pos, m_Uv_size, m_Type);
			}

			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
		};
	}
}