#include "map.h"


namespace hiraeth {
	namespace map {

		Map::Map(const std::string& filename, int map_index, graphics::Window* wind)
			: 
		m_PtTex("portal_adv.png"),
			m_PtShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_BgShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_CrShader("src/shaders/basic.vert", "src/shaders/basic.frag"),
			m_PtLayer(&m_PtShader),
			m_BgLayer(&m_BgShader),
			m_CrLayer(&m_CrShader),
			m_MapLayer(filename),
			m_MapIndex(map_index), 
			m_Wnd(wind), m_Char(maths::vec2(0, 0), wind->getKeyboard(), getMapLayer()) 
		{
			graphics::Label* fps = new graphics::Label("arial", 50, "hiraeth", 400, 0, 0xffff0000);
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, new graphics::Texture("bg1.png")));
			m_CrLayer.add_ref(&m_Char);
			m_BgLayer.add(fps);
			m_Wnd->getKeyboard()->registerToKey(input::Controls::up, this);
			view::Camera::init(&m_Char);
			if (map_index == 2)
			{
				map_index = 0;
				serialize_map_data();
				deserialize_map_data(map_index);
			}
			else
				deserialize_map_data(map_index);
		}

		Map::~Map()
		{
		}

		void Map::draw()
		{
			m_BgLayer.render();

			m_MapLayer.draw();

			m_CrShader.enable();
			m_CrShader.setUniformMat4("pr_matrix", view::Camera::get_ortho());
			m_CrLayer.render();

			m_PtShader.enable();
			m_PtShader.setUniformMat4("pr_matrix", view::Camera::get_ortho());
			m_PtLayer.render();
			
		}

		void Map::update()
		{
			m_CrLayer.update();
			m_PtLayer.update();
			if (m_ChangeMapFlag) 
			{
				deserialize_map_data(m_MapIndex);
			}
		}

		void Map::change_map(int new_index)
		{
			m_MapIndex = new_index;
			m_ChangeMapFlag = true;
		}

		void Map::ButtonClicked(input::Controls control)
		{
			if (input::Controls::up == control)
				for (auto& portal : m_MapData.m_Portals)
					if (portal->getBounds().GetMinimumBound().Distance(m_Char.getPosition()) <= 100)
						change_map(portal->getNextMap());
		}

		void Map::serialize_map_data()
		{
			MapData map_data1{}, map_data2{};

			map_data1.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(-800, -100), maths::vec2(800, -100)));
			for (int i = 0; i < 2; i++)
				map_data1.m_Portals.push_back(std::make_unique<Portal>(maths::vec3(i * 200, 0, 0), 1));
			for (int y = -450; y < -100; y += 59)
				for (int x = -800; x < 800; x += 89)
					map_data1.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(x, y, 0), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0 ));
			map_data1.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(-90, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			map_data1.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(0, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			map_data1.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(-190, 0,0), 1.0f, maths::vec2(1, 268), maths::vec2(90, 59), 0));
			map_data1.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(0, 0,0), 1.0f, maths::vec2(0, 0), maths::vec2(621, 328), 0));
			map_data1.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(-800, -100), maths::vec2(800,-100)));
			map_data1.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(800, -150), maths::vec2(1000,-150)));
			map_data1.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(800, -50), maths::vec2(1000,-50)));
			//map_data1.m_FootHolds.push_back(physics::FootHold(maths::vec2(-800, 0), maths::vec2(800,-200), maths::vec2(0, 1)));
			map_data1.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(1000, -200), 600));
			map_data1.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(-700, -200), 600));
			map_data1.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(-800, -450), 350));
			map_data1.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(800, -450), 350));

			for (int i = 0;i < 2; i++)
				map_data2.m_Portals.push_back(std::make_unique<Portal>( maths::vec3((i * (-200)) - 200, 0,0), 0 ));
			for (int y = -350; y < 0; y += 59)
				for (int x = -1000; x < 600; x += 89)
					map_data2.m_Tiles.push_back(std::make_unique<Tile>(maths::vec3(x, y, 0), 1.0f, maths::vec2(1 + (rand() % 6) * 91, 268), maths::vec2(90, 59), 0));
			map_data2.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(-800, -100), maths::vec2(800,-100)));
			map_data2.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(800, -150), maths::vec2(1000,-150)));
			map_data2.m_FootHolds.push_back(physics::HorizontalFootHold(maths::vec2(800, -50), maths::vec2(1000,-50)));
			//map_data2.m_FootHolds.push_back(physics::FootHold(maths::vec2(-800, 0), maths::vec2(800,-200), maths::vec2(0, 1)));
			map_data2.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(1000, -200), 600));
			map_data2.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(-700, -200), 600));
			map_data2.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(-800, -450), 350));
			map_data2.m_FootHolds.push_back(physics::VerticalFootHold(maths::vec2(800, -450), 350));

			std::ofstream file("map.data");
			cereal::BinaryOutputArchive oarchive(file);
			//oarchive(int(2));
			file.seekp(sizeof(int) * 2);
			int map_data1_location = file.tellp();
			oarchive(map_data1);
			int map_data2_location = file.tellp();
			oarchive(map_data2);
			file.seekp(0);
			oarchive(map_data1_location);
			oarchive(map_data2_location);
		}

		void Map::deserialize_map_data(unsigned int map_index)
		{
			std::ifstream file("map.data");
			cereal::BinaryInputArchive iarchive(file);
			int start_of_data;
			file.seekg(sizeof(int) * map_index);
			iarchive(start_of_data);
			file.seekg(start_of_data);
			iarchive(m_MapData);
			m_MapLayer.m_Tiles = &m_MapData.m_Tiles;
			m_MapLayer.m_FootHolds = &m_MapData.m_FootHolds;
			m_PtLayer.clear();
			for (auto& tile : m_MapData.m_Portals)
				m_PtLayer.add_ref(tile.get());
			m_ChangeMapFlag = false;
		}

	}
}
