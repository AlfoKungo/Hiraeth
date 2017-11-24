#include "tile.h"

namespace hiraeth {
	namespace map {
		 //Tile::Tile(maths::vec3 pos, float scale, unsigned int type)
			// : MapRenderable(maths::vec3(pos.x,pos.y,0), uv_size * scale, 
			//	 create_uv_by_pos_size(uv_pos, uv_size, maths::vec2(621,328))),
			// m_Scale(scale), m_Uv_pos(uv_pos), m_Uv_size(uv_size), m_Type(type)
		 Tile::Tile(maths::vec3 pos, float scale, unsigned int type)
			 : MapRenderable(maths::vec3(pos.x,pos.y,0), type),
			 m_Scale(scale)
		 {

		 }

		 Tile::Tile(SRL::TileData s)
			 : Tile(s.position, s.scale, s.type)
		 {

		 }

		 Tile::~Tile()
		 {

		 }

		//std::vector<maths::vec2> Tile::create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size)
		//{
		//	std::vector<maths::vec2> uv;
		//	uv.push_back((pos) / tex_size);
		//	uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
		//	uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
		//	uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
		//	return uv;
		//}
	}
}