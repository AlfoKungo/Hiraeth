#pragma once
#include "graphics/layers/layer.h"
#include "npc.h"
#include "item/item_manager.h"
#include "game/monster_manager.h"

namespace hiraeth
{
	namespace game
	{
		class Character;

		class NpcManager : public Updatable
		{

			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<npc::Npc> m_Layer;
			input::Keyboard* m_Kb;
		public:
			NpcManager(map::MapLayer* map_layer, input::Keyboard* kb);
			void draw() const;
			void update() override;
			void mapChanged();
		};
	}
}
