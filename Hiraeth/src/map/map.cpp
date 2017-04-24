#include "map.h"

 
namespace hiraeth {
	namespace map {

		Map::Map(const std::string& filename, int map_index, graphics::Window* wind, view::Camera* camera, Timer* time)
			: m_Char(maths::vec3(-230, 0, 0),time, wind->getKeyboard(), &m_MapLayer),
			m_PtTex("portal_adv.png"),
			m_BgShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_PtShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_CrShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_BgLayer(&m_BgShader),
			m_PtLayer(&m_PtShader),
			m_CrLayer(&m_CrShader),
			//m_BgLayer(&m_BgShader),
			//m_PtLayer(&m_BgShader),
			//m_CrLayer(&m_BgShader),
			m_MapLayer(filename, camera), 
			m_Wnd(wind), m_Time(time), m_Camera(camera), m_MapIndex(map_index) 
		{
			graphics::Label* fps = new graphics::Label("arial", 50, "hiraeth", 400, 0, 0xffff0000);
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, new graphics::Texture("bg1.png")));
			m_CrLayer.add_ref(&m_Char);
			m_BgLayer.add(fps);
			camera->setCharacter(&m_Char);
			//m_CrLayer.add(new game::Character(maths::vec3(0, 0, 0), new graphics::Texture("char.png"), time, wind));
			if (map_index == 2)
			{
				serialize();
				map_index = 0;
			}
			deserialize(map_index);
		}

		Map::~Map()
		{
			m_MapLayer.clear();
			m_PtLayer.clear();
		}

		void Map::draw()
		{
			m_BgLayer.render();

			m_MapLayer.draw();

			m_CrShader.enable();
			m_CrShader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_CrLayer.render();

			m_PtShader.enable();
			m_PtShader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_PtLayer.render();
			
		}

		void Map::update()
		{
			m_Char.update();
			//m_CrLayer.update();
			m_PtLayer.update();
			if (m_ChangeMapFlag) 
			{
				deserialize(m_MapIndex);
			}
		}

		void Map::change_map(int new_index)
		{
			m_MapIndex = new_index;
			m_ChangeMapFlag = true;
		}

		void Map::serialize()
		{
			std::ofstream file("map.hi");
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
			oarchive(Tile::Serializer{maths::vec3(-190, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0});
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
			std::ifstream file("map.hi");
			cereal::BinaryInputArchive iarchive(file);
			m_MapLayer.clear();
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
			m_MapLayer.deserialize_tiles(&iarchive);
			m_ChangeMapFlag = false;
		}

		void Map::deserialize_portals(cereal::BinaryInputArchive* iarchive)
		{
			Portal::Serializer s;
			int portals_amount;
			(*iarchive)(portals_amount);
			for (int i = 0; i < portals_amount; i++)
			{
				(*iarchive)(s);
				m_PtLayer.add(new Portal(s, m_Wnd->getKeyboard(), &m_PtTex, this, m_Time, &m_Char));
			}
		}

	}
}