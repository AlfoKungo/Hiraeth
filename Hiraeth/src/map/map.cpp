#include "map.h"


namespace hiraeth {
	namespace map {

		Map::Map(int map_index, graphics::Window* wind)
			:
			m_PtTex("portal_adv.png"),
			m_PtShader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_BgShader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_PtLayer(&m_PtShader),
			m_BgLayer(&m_BgShader),
			m_MapLayer(),
			m_MapIndex(map_index),
			m_Wnd(wind)
		{
			graphics::Label* fps = new graphics::Label("arial", 50, "hiraeth", 400, 0, 0xffff0000);
			m_BgLayer.add(new graphics::Sprite(-900, -450, 1920, 1080, new graphics::Texture("bg.png", 0)));
			m_BgLayer.add(fps);
			m_Wnd->getKeyboard()->registerToKey(input::Controls::up, this);

			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->createEvent(MapChanged);

			deserialize_map_data(map_index);
		}

		Map::~Map()
		{
		}

		void Map::draw()
		{
			m_BgLayer.render();

			m_MapLayer.draw();

			m_PtShader.enable();
			m_PtShader.setUniformMat4("pr_matrix", view::Camera::get_ortho());
			m_PtLayer.render();
			
		}

		void Map::update()
		{
			m_PtLayer.update();
			if (m_ChangeMapFlag) 
			{
				deserialize_map_data(m_MapIndex);
				EventManager *m_EventManager = EventManager::Instance();
				m_EventManager->execute(MapChanged);
			}
		}

		void Map::change_map(int new_index)
		{
			m_MapIndex = new_index;
			m_ChangeMapFlag = true;
		}

		void Map::ButtonClicked(input::Controls control)
		{
			maths::vec2 char_pos = view::Camera::getCharacterPos();
			if (input::Controls::up == control)
				//for (auto& portal : m_MapLayer.getPortals())
				for (auto& portal : m_PtLayer.m_Renderables)
					if (portal->getBounds().GetMinimumBound().Distance(char_pos) <= 100)
						change_map(portal->getNextMap());
		}

		void Map::deserialize_map_data(unsigned int map_index)
		{

			m_MapLayer.reloadData(map_index);

			m_PtLayer.clear();
			for (auto& portal : m_MapLayer.m_MapData.Portals)
				m_PtLayer.add(new Portal(portal));

			m_ChangeMapFlag = false;
		}
	}
}
