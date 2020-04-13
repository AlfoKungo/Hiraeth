#pragma once
#include <winsock2.h>
#include <Ws2tcpip.h>

#include "graphics/layers/layer.h"
#include "npc.h"
#include "item/item_manager.h"
#include "game/monsters/monster_manager.h"
#include "dialog_manager.h"
#include "UI/ui_quests.h"


namespace hiraeth
{
	namespace game
	{

		class NpcManager : public Updatable, public input::MouseEvent
		{

			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<npc::Npc> m_Npcs;
			input::Keyboard* m_Kb;
			npc::DialogManager m_DialogManager;
			ui::UiQuests * m_UiQuests;
			network::ClientHandler * m_ClientHandler;
		public:
			NpcManager(map::MapLayer* map_layer, input::Keyboard* kb, 
				game::Character * character, ui::UiQuests * ui_quests);
			void draw() const;
			void update() override;
			void mapChanged();
			void sendStartDialog(unsigned int npc_index);


			// MOUSE EVENT FUNCTIONS
			bool leftButtonClicked(float mx, float my) override;
			bool leftButtonReleased(float mx, float my)  override;
			bool rightButtonClicked(float mx, float my) override;
			bool mouseMove(float pmx, float pmy, float mx, float my)  override;
			bool is_window_contains(maths::vec2 mouse_pos) const override;
		};
	}
}
