#pragma once

#include "utils/timer.h"
#include <vector>
#include "tile.h"
#include "../graphics/texture.h"
#include "../graphics/layers/tilelayer.h"
#include "../graphics/shader.h"
#include "../maths/maths.h"
#include "../graphics/sprite.h"
#include "../graphics/window.h"

#include "maprenderer.h"
#include "portal.h"
#include "../graphics/renderer.h"
#include "../graphics/sprited_renderable.h"
#include "../game/character.h"
#include "../game/monster.h"
#include "../view/camera.h"
#include "../basic/drawable.h"
#include "../basic/updatable.h"
#include "../graphics/label.h"
#include "../graphics/font_manager.h"
#include "map_data.h"

#include "maplayer.h"

#include <cereal/archives/binary.hpp>
#include <fstream>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

namespace hiraeth {
	namespace map {

		class Map :  Updatable, public input::KeyboardEvent
		{
		private:

			graphics::Texture m_PtTex;

			graphics::Shader m_PtShader;
			graphics::Shader m_BgShader;
			graphics::Shader m_CrShader;


			graphics::Layer<Portal> m_PtLayer;
			graphics::Layer<graphics::Renderable> m_BgLayer;
			graphics::Layer<graphics::Renderable> m_CrLayer;

			MapData m_MapData{};

			MapLayer m_MapLayer;

			int m_MapIndex;
			bool m_ChangeMapFlag;

			graphics::Window* m_Wnd;
			game::Character m_Char;
		public:
			Map(const std::string& filename, int map_index, graphics::Window* wind);
			~Map();

			void draw();
			void update() override;
			void change_map(int new_index);
			MapLayer* getMapLayer() { return &m_MapLayer; }
			game::Character* getCharacter() { return &m_Char; }

			void ButtonClicked(input::Controls control) override;
			void ButtonReleased(input::Controls control) override {}

		private:
			void serialize_map_data() const;
			void deserialize_map_data(unsigned int map_index);
		};

	}
}