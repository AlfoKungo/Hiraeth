#include "npc_manager.h"

namespace hiraeth
{
	namespace game
	{
		NpcManager::NpcManager(map::MapLayer* map_layer, input::Keyboard* kb)
			: m_MapLayer(map_layer),
			  m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			  m_Layer(&m_Shader, true),
			  m_Kb(kb)
		{
			EventManager* m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &NpcManager::mapChanged);
			mapChanged();
		}

		void NpcManager::draw() const
		{
			m_Layer.render();
		}

		void NpcManager::update()
		{
			m_Layer.update();
		}

		void NpcManager::mapChanged()
		{
			//clear summon queue
			m_Layer.clear();
			//m_Layer.add(new npc::Npc(0));
			//reload new summons
			for (auto npc_index : (m_MapLayer->getNpcs()))
				m_Layer.add(new npc::Npc{npc_index, m_MapLayer, m_Kb});
		}
	}
}
