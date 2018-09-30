#include "monster_manager.h"

namespace hiraeth {
	namespace game {

		MonsterManager::MonsterManager(map::MapLayer* map_layer, game::Character* character, item::ItemManager* item_manager)
			: m_MapLayer(map_layer), 
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), 
			m_Layer(&m_Shader, true),
			m_ItemManager(item_manager),
			m_Char(character)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &MonsterManager::mapChanged);
			mapChanged();
			m_Char->setMonsters(&m_Layer.m_Renderables);
			std::srand(std::time(nullptr));
		}

		void MonsterManager::draw() const
		{
			m_Layer.render();
		}

		void MonsterManager::update()
		{
			m_Layer.update();

			//destroying dead monsters
			for (auto monster = m_Layer.m_Renderables.begin(); monster != m_Layer.m_Renderables.end();)
			{
				if ((*monster)->has_died)
				{
					m_ItemManager->dropItem((*monster)->getBounds().GetBottomMiddle(), (unsigned int)(rand() % 2) + 3);
					SRL::Summon summon = (*monster)->getSummon();
					delete (*monster);
					monster = m_Layer.m_Renderables.erase(monster);
					m_SummonQueue.push(Summoner{summon, StaticTimer::timer.elapsed() + 1.0f});
				}
				else
				{
					++monster;
				}
			}

			//resummoning dead monsters
			while (!m_SummonQueue.empty())
			{
				if (StaticTimer::timer.elapsed() - m_SummonQueue.front().summonTime > 0.0f)
				{
					m_Layer.add(new Monster(m_SummonQueue.front().summon, m_MapLayer));
					m_SummonQueue.pop();
				}
				else
					break;
			}

			checkCollision();
		}

		void MonsterManager::checkCollision()
		{
			if (!m_Char->is_hit)
				for (auto monster : m_Layer.m_Renderables)
				{
					if (monster->check_collision(m_Char->getHitBox()))
					{
						std::cout << "colliding" << std::endl;
						if (monster->getBounds().GetBottomMiddle().x < m_Char->getBounds().GetBottomMiddle().x)
							m_Char->getHit(game::Direction::Right, monster->getDamage());
						else
							m_Char->getHit(game::Direction::Left, monster->getDamage());
						return;
					}
				}
		}

		void MonsterManager::mapChanged()
		{
			//clear summon queue
			while (!m_SummonQueue.empty())
				m_SummonQueue.pop();
			m_Layer.clear();
			MonsterDataManager::ReloadData(m_MapLayer->getSummons());
			//reload new summons
			for (auto summon : (m_MapLayer->getSummons()))
				m_SummonQueue.push(Summoner{ summon, StaticTimer::timer.elapsed() + 2.0f});
		}
	}
}
