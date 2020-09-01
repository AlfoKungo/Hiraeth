#pragma once

#include "../graphics/texture.h"
#include "../graphics/layers/tilelayer.h"
#include "../graphics/shader.h"
#include "../graphics/sprite.h"
#include "../graphics/window.h"
#include "basic/EventManager.h"
#include "graphics/label.h"
#include "basic/network_handler.h"

#include "portal.h"
#include "../basic/updatable.h"

#include "maplayer.h"

namespace hiraeth {
	namespace map {

		class Map :  Updatable, public input::KeyboardEvent
		{
		private:
			const unsigned int up = 0;

			graphics::Texture m_PtTex;

			graphics::Shader m_PtShader;
			graphics::Shader m_BgShader;

			graphics::Layer<Portal> m_PtLayer;
			graphics::Layer<graphics::Renderable> m_BgLayer;

			MapLayer m_MapLayer;

			int m_MapIndex;
			//bool m_ChangeMapFlag;


			graphics::Window* m_Wnd;
		public:
			Map(int map_index, graphics::Window* wind);
			~Map();

			void draw();
			void update() override;
			void change_map(int new_index);
			MapLayer* getMapLayer() { return &m_MapLayer; }

			void ButtonClicked(input::Key control) override;
			void ButtonReleased(input::Key control) override {}

		private:
			void deserialize_map_data(unsigned int map_index);
		};

	}
}
