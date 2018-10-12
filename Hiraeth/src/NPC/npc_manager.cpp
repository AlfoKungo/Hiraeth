#include "npc_manager.h"

namespace hiraeth
{
	namespace game
	{
		NpcManager::NpcManager(map::MapLayer* map_layer, input::Keyboard* kb)
			: m_MapLayer(map_layer),
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_Npcs(&m_Shader, true),
			m_Kb(kb)
		{
			EventManager* m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &NpcManager::mapChanged);
			mapChanged();
			kb->registerToMouse(this);
		}

		void NpcManager::draw() const
		{
			m_Npcs.render();
			m_DialogManager.draw();
		}

		void NpcManager::update()
		{
			m_Npcs.update();
			m_DialogManager.update();
		}

		void NpcManager::mapChanged()
		{
			//clear summon queue
			m_Npcs.clear();
			//reload new summons
			for (auto npc_index : (m_MapLayer->getNpcs()))
				m_Npcs.add(new npc::Npc{ npc_index, m_MapLayer, m_Kb });
		}

		bool NpcManager::leftButtonClicked(float mx, float my) 
		{
			const maths::vec2 translated_pos = view::Camera::mouse_to_map_position(maths::vec2(mx, my));
			for (const auto& npc : m_Npcs.m_Renderables)
				if (npc->getBounds().Contains(translated_pos))
				{
					npc->onNpcClick();
					m_DialogManager.nextDialog();
				}
			return false;
		}
		bool NpcManager::rightButtonClicked(float mx, float my) 
		{
			return false; 
		}
		bool NpcManager::leftButtonReleased(float mx, float my) const
		{
			return false;
		}
		bool NpcManager::mouseMove(float pmx, float pmy, float mx, float my) const
		{
			return false;
		}

		bool NpcManager::is_window_contains(maths::vec2 mouse_pos) const
		{
			maths::vec2 translated_pos = view::Camera::mouse_to_map_position(mouse_pos);
			auto result_npc = std::find_if(std::begin(m_Npcs.m_Renderables), std::end(m_Npcs.m_Renderables),
				[&](auto const& npc) 
			{ return npc->is_to_draw && npc->getBounds().Contains(translated_pos); });
			return result_npc != m_Npcs.m_Renderables.end();
		}

	}
}
