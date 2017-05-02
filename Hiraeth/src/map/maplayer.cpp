#include "maplayer.h"

namespace hiraeth {
	namespace map {

		MapLayer::MapLayer(const std::string& filename, view::Camera* camera)
			: m_Camera(camera),
			m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_Renderer(new graphics::Texture(filename))
		{

		}
		MapLayer::~MapLayer()
		{
			for (unsigned int i = 0; i < m_Tiles.size(); i++)
			{
					delete m_Tiles[i];
			}
			m_Tiles.clear();
		}

		void MapLayer::draw()
		{
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_Tex.getID());
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_Renderer.begin();
			for (const Tile* tile : m_Tiles)
				tile->submit(m_Renderer);
			m_Renderer.end();
			m_Renderer.flush();
		}

		void MapLayer::add(Tile::Serializer s)
		{
			m_Tiles.push_back(new Tile(s));
		}
		void MapLayer::deserialize_tiles(cereal::BinaryInputArchive* iarchive)
		{
			Tile::Serializer s;
			int tile_amount;
			(*iarchive)(tile_amount);
			for (int i = 0; i < tile_amount; i++)
			{
				(*iarchive)(s);
				m_Tiles.push_back(new Tile(s));
			}
			m_FootHolds.push_back(physics::FootHold(maths::vec2(-800, -100), maths::vec2(800,-100), maths::vec2(0, 1)));
			//m_FootHolds.push_back(physics::FootHold(maths::vec2(-800, 0), maths::vec2(800,-200), maths::vec2(0, 1)));
			m_FootHolds.push_back(physics::FootHold(maths::vec2(-700, -200), maths::vec2(-700,400), maths::vec2(1, 0)));
			m_FootHolds.push_back(physics::FootHold(maths::vec2(-800, -450), maths::vec2(-800,-100), maths::vec2(1, 0)));
			m_FootHolds.push_back(physics::FootHold(maths::vec2(800, -450), maths::vec2(800,-100), maths::vec2(1, 0)));
		}

		void MapLayer::clear()
		{
			for (unsigned int i = 0; i < m_Tiles.size(); i++)
			{
					delete m_Tiles[i];
			}
			m_Tiles.clear();
		}
	}
}
