#pragma once
#include "graphics/layers/layer.h"
#include "npc.h"
#include "item/item_manager.h"
#include "game/monster_manager.h"
//#include "view/camera.h"

namespace hiraeth
{
	namespace game
	{
		class Character;

		class NpcManager : public Updatable, public input::MouseEvent
		{

			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<npc::Npc> m_Npcs;
			input::Keyboard* m_Kb;
		public:
			NpcManager(map::MapLayer* map_layer, input::Keyboard* kb);
			void draw() const;
			void update() override;
			void mapChanged();

			// MOUSE EVENT FUNCTIONS
			bool leftButtonClicked(float mx, float my) override;
			bool leftButtonReleased(float mx, float my) const override;
			bool rightButtonClicked(float mx, float my) override;
			bool mouseMove(float pmx, float pmy, float mx, float my) const override;
			bool is_window_contains(maths::vec2 mouse_pos) const override;
		};
	}
}
