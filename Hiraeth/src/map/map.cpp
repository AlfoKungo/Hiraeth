#include "map.h"

 
namespace hiraeth {
	namespace map {

		Map::Map(const std::string& filename, const std::string& bg_filename, std::vector<Tile> tiles, view::Camera* camera)
			: m_Tex(filename), m_BgTex(bg_filename), m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_BgLayer(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag")),
			m_Camera(camera), m_Renderer(filename)
		{
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, &m_BgTex));
			for (std::vector<Tile>::const_iterator it = tiles.begin(); it != tiles.end(); it++)
				m_Tiles.push_back(*it);

			//glActiveTexture(GL_TEXTURE0);
		}
		Map::Map(const std::string& filename, const std::string& bg_filename, int offset, view::Camera* camera)
			: m_Tex(filename), m_BgTex(bg_filename), m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_BgLayer(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag")),
			m_Camera(camera), m_Renderer(filename)
		{
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, &m_BgTex));
			deserialize_tiles(offset);

			//glActiveTexture(GL_TEXTURE0);
		}

		Map::~Map()
		{

		}

		void Map::draw()
		{
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_Shader.disable();

			m_BgLayer.render();

			m_Tex.bind();
			m_Shader.enable();
			m_Renderer.begin();

			for (const Tile tile : m_Tiles)
				tile.submit(&m_Renderer);

			m_Renderer.end();

			m_Renderer.flush();
		}

		void Map::serialize_tiles()
		{
			std::vector<Tile> tiles1, tiles2;
			for (int y = -450; y < -100; y += 59)
				for (int x = -800; x < 800; x += 89)
					tiles1.push_back(Tile(maths::vec2(x, y), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0));
			tiles1.push_back(Tile(maths::vec2(-90, 0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			tiles1.push_back(Tile(maths::vec2(0, 0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			tiles1.push_back(Tile(maths::vec2(-180, 0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			tiles1.push_back(Tile(maths::vec2(270, 0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			tiles1.push_back(Tile(maths::vec2(0, 0), 1.0f, maths::vec2(0, 0), maths::vec2(621, 328), 0));
			for (int y = -350; y < 0; y += 59)
				for (int x = -1000; x < 600; x += 89)
					tiles2.push_back(Tile(maths::vec2(x, y), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0));

			std::ofstream file("my_file.save");
			cereal::BinaryOutputArchive oarchive(file);
			oarchive((int)2);
			file.seekp(sizeof(int) + sizeof(int)*2);
			for (std::vector<Tile>::const_iterator it = tiles1.begin(); it != tiles1.end(); it++)
			{
				std::cout << file.tellp() << std::endl;
				//m1.reset(&(*it));
				//std::unique_ptr<Tile const> const m1{ new Tile(maths::vec2(-90, 0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0) };
				std::unique_ptr<Tile const> m1{ &*it };
				oarchive(m1);
				m1.release();
			}
			int loc1 = file.tellp();
				std::cout << loc1 << std::endl;
			file.seekp(sizeof(int));
			oarchive(loc1 - sizeof(int) * 3);
			file.seekp(loc1);
			for (std::vector<Tile>::const_iterator it = tiles2.begin(); it != tiles2.end(); it++)
			{
				std::cout << file.tellp() << std::endl;
				std::unique_ptr<Tile const> m1{ &*it };
				oarchive(m1);
				m1.release();
			}
			int loc2 = file.tellp();
				std::cout << loc2 << std::endl;
			file.seekp(sizeof(int) + sizeof(int));
			oarchive(loc2 - loc1 - sizeof(int) * 3);
		}

		void Map::deserialize_tiles(int offset)
		{
			std::ifstream file("my_file.save");
			cereal::BinaryInputArchive iarchive(file);
			int amount;
			iarchive(amount);
			int beg_of_data, tmp, end_of_data = 0;
			for (int i = 0; i <= offset; i++)
			{
				std::cout << file.tellg() << std::endl;
				iarchive(tmp);
				end_of_data += tmp;
			}
			beg_of_data = end_of_data - tmp;
			file.seekg(sizeof(amount) + amount * sizeof(int) + beg_of_data);
			std::unique_ptr<Tile> deserialized_message{ nullptr };
			for (int i = 0; file.tellg() < end_of_data; i++)
			{
				iarchive(deserialized_message);
				//m_Tiles.push_back(*(deserialized_message.get()));
				m_Tiles.push_back(Tile(*deserialized_message.get()));
			}
		}
	}
}