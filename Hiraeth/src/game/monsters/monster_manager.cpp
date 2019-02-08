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

			////destroying dead monsters
			//for (auto monster = m_Layer.m_Renderables.begin(); monster != m_Layer.m_Renderables.end();)
			//{
			//	if ((*monster)->has_died)
			//	{
			//		m_ItemManager->dropItem((*monster)->getBounds().GetBottomMiddle(), (unsigned int)(rand() % 2) + 3);
			//		SRL::Summon summon = (*monster)->getSummon();
			//		delete (*monster);
			//		monster = m_Layer.m_Renderables.erase(monster);
			//		m_SummonQueue.push(Summoner{summon, StaticTimer::timer.elapsed() + 1.0f});
			//	}
			//	else
			//	{
			//		++monster;
			//	}
			//}

			//checkCollision();
		}

		//void MonsterManager::addMonster(unsigned monster_id, unsigned summon_index)
		//{
		//	m_Layer.add(new Monster(m_SummonQueue.front().summon, m_MapLayer));
		//}

		void MonsterManager::addMonster(unsigned monster_id, network::MonsterStateUpdate monster_state)
		{
			if (m_Monsters.find(monster_id) == m_Monsters.end())
			{
				const auto new_monster = new Monster{ monster_state.monster_type, monster_state.pos, m_MapLayer };
				m_Monsters[monster_id] = new_monster;
				m_Layer.add(new_monster);
			}
			updateMonster(monster_id, monster_state);
		}

		void MonsterManager::updateMonster(unsigned monster_id, network::MonsterStateUpdate monster_state)
		{
			//printf("received some data (%f, %f), (%d, %d)", monster_state.pos.x, monster_state.pos.y, monster_state.left, monster_state.right);
			printf("received some data (%f, %f), (%d)\n", monster_state.pos.x, monster_state.pos.y, monster_state.dir);
			m_Monsters[monster_id]->setPosition(monster_state.pos);
			m_Monsters[monster_id]->setControls(0, monster_state.dir == network::Left);
			m_Monsters[monster_id]->setControls(1, monster_state.dir == network::Right);
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
			/*
			 * add rereading of data from server.
			 */

			//clear summon queue
			//while (!m_SummonQueue.empty())
			//	m_SummonQueue.pop();
			//m_Layer.clear();
			//MonsterDataManager::ReloadData(m_MapLayer->getSummons());
			////reload new summons
			//for (auto summon : (m_MapLayer->getSummons()))
			//	m_SummonQueue.push(Summoner{ summon, StaticTimer::timer.elapsed() + 2.0f});
		}
	}
}
