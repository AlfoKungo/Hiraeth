#include "map.h"

 
namespace hiraeth {
	namespace map {

		Map::Map(const std::string& filename, int map_index, graphics::Window* wind, view::Camera* camera)
			: m_Tex(filename),
			m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_BgShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_PtShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_BgLayer(&m_BgShader), m_PtLayer(&m_PtShader), m_Wnd(wind),
			m_Camera(camera), m_Renderer(filename), m_MapIndex(map_index)
		{
			//m_PtLayer.add(new Portal(maths::vec3(1, 1, 1), 1, wind, new graphics::Texture("portal_adv.png"), this));
			//m_PtLayer.add(new Portal(maths::vec3(500, 0, 1), 1, wind, new graphics::Texture("portal.png"), this));
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, new graphics::Texture("bg1.png")));
			if (map_index == 2)
			{
				serialize();
				map_index = 0;
			}
			deserialize(map_index);
			//GLint texIDs[] =
			//{
			//	0,1,2,3,4,5,6,7,8,9
			//};
			//m_BgShader.enable();
			//m_BgShader.setUniform1iv("textures", texIDs, 10);
			//m_PtShader.enable();
			//m_PtShader.setUniform1iv("textures", texIDs, 10);

		}

		Map::~Map()
		{
		}

		void Map::draw()
		{
			m_BgLayer.render();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Tex.getID());
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_Renderer.begin();
			for (const Tile tile : m_Tiles)
				tile.submit(&m_Renderer);
			m_Renderer.end();
			m_Renderer.flush();

			m_PtShader.enable();
			m_PtShader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_PtLayer.render();
		}

		void Map::update()
		{

			m_PtLayer.update();

			if (m_ChangeMapFlag) 
			{
				deserialize(m_MapIndex);
				m_ChangeMapFlag = false;
			}
		}

		void Map::change_map(int new_index)
		{
			m_MapIndex = new_index;
			m_ChangeMapFlag = true;
		}

		void Map::serialize()
		{
			std::ofstream file("my_file.save");
			cereal::BinaryOutputArchive oarchive(file);
			oarchive((int)2);
			file.seekp(sizeof(int) + sizeof(int)*2);
			oarchive(2);
			for (int i = 0;i < 2; i++)
			{
				Portal::Serializer s{ maths::vec3(i * 200, 0,0), 1 };
				oarchive(s);
			}
			int count = 0, tile_count_pos = file.tellp();
			file.seekp(sizeof(int), std::ios::cur);
			for (int y = -450; y < -100; y += 59)
				for (int x = -800; x < 800; x += 89)
				{
					oarchive(Tile::Serializer{ maths::vec3(x, y, 0), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0 });
					//std::cout << file.tellp() << std::endl;
					count++;
				}
			oarchive(Tile::Serializer{maths::vec3(-90, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0});
			oarchive(Tile::Serializer{maths::vec3(0, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0});
			oarchive(Tile::Serializer{maths::vec3(-180, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0});
			oarchive(Tile::Serializer{maths::vec3(0, 0,0), 1.0f, maths::vec2(0, 0), maths::vec2(621, 328), 0});
			int loaction_after1 = file.tellp();
			file.seekp(tile_count_pos);
			oarchive(count + 4);
			file.seekp(sizeof(int));
			oarchive(loaction_after1 - sizeof(int) * 3);

			file.seekp(loaction_after1);
			oarchive(2);
			for (int i = 0;i < 2; i++)
			{
				Portal::Serializer s{ maths::vec3((i * (-200)) - 200, 0,0), 0 };
				oarchive(s);
			}
			count = 0;
			tile_count_pos = file.tellp();
			file.seekp(sizeof(int), std::ios::cur);
			for (int y = -350; y < 0; y += 59)
				for (int x = -1000; x < 600; x += 89)
				{
					oarchive(Tile::Serializer{ maths::vec3(x, y, 0), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0 });
					count++;
				}
			int location_after2 = file.tellp();
			file.seekp(tile_count_pos);
			oarchive(count);
			file.seekp(sizeof(int) + sizeof(int));
			oarchive(location_after2 - loaction_after1 - sizeof(int) * 3);
		}

		void Map::deserialize(int map_index)
		{
			std::ifstream file("my_file.save");
			cereal::BinaryInputArchive iarchive(file);
			m_Tiles.clear();
			m_PtLayer.clear();

			int maps_amount, start_of_data = 0, tmp;
			//read maps_amount
			iarchive(maps_amount);
			//read start_of_data and end_of_data
			for (int i = 0; i < map_index; i++)
			{
				iarchive(tmp);
				start_of_data += tmp;
			}
			file.seekg(sizeof(maps_amount) + maps_amount * sizeof(int) + start_of_data);
			//deserialize tiles
			deserialize_portals(&iarchive);
			deserialize_tiles(&iarchive);
		}

		void Map::deserialize_portals(cereal::BinaryInputArchive* iarchive)
		{
			Portal::Serializer s;
			int portals_amount;
			(*iarchive)(portals_amount);
			for (int i = 0; i < portals_amount; i++)
			{
				(*iarchive)(s);
				m_PtLayer.add(new Portal(s, 110, m_Wnd, new graphics::Texture("portal_adv.png"), this));
			}
		}

		void Map::deserialize_tiles(cereal::BinaryInputArchive* iarchive)
		{
			Tile::Serializer s;
			int tile_amount;
			(*iarchive)(tile_amount);
			for (int i = 0; i < tile_amount; i++)
			{
				(*iarchive)(s);
				m_Tiles.push_back(Tile(s));
			}
		}
	}
}