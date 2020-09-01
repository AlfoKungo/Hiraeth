#include "monster_manager.h"

namespace hiraeth {
	namespace game {

		MonsterManager::MonsterManager(map::MapLayer* map_layer)
			: m_MapLayer(map_layer),
			m_Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"),
			m_Layer(&m_Shader, true)
			//m_ItemManager(item_manager)
			//m_Char(character)
		{
			EventManager* m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &MonsterManager::mapChanged);
			m_EventManager->createEvent<unsigned int>(MonsterDied);
			mapChanged();
			//m_Char->setMonsters(&m_Layer.m_Renderables);
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
					auto monster_id = (*monster)->getId();
					m_Monsters.erase((*monster)->getId());
					delete (*monster);
					monster = m_Layer.m_Renderables.erase(monster);
					EventManager* m_EventManager = EventManager::Instance();
					m_EventManager->execute<unsigned int>(MonsterDied, monster_id);
					//++monster;
				}
				else
				{
					++monster;
				}
			}

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
				auto x_start = m_MapLayer->m_MapData.Summons[monster_id].XBegin, x_end = m_MapLayer->m_MapData.Summons[monster_id].XEnd;
				const auto new_monster = new Monster{ monster_state.monster_type, monster_state.pos, m_MapLayer, monster_id ,
				x_start, x_end };
				m_Monsters[monster_id] = new_monster;
				m_Layer.add(new_monster);
			}
			updateMonster(monster_id, monster_state);
		}

		void MonsterManager::updateMonster(unsigned monster_id, network::MonsterStateUpdate monster_state)
		{
			//printf("received some data (%f, %f), (%d, %d)", monster_state.pos.x, monster_state.pos.y, monster_state.left, monster_state.right);
			printf("received some data (%f, %f), (%d)\n", monster_state.pos.x, monster_state.pos.y, monster_state.dir);
			if (m_Monsters.find(monster_id) == m_Monsters.end())
			{
				addMonster(monster_id, monster_state);
			}
			std::cout << "id is " << monster_id << " org pos is " << m_Monsters[monster_id]->getPosition() << " new_pos is " << monster_state.pos << std::endl;
			//m_Monsters[monster_id]->setPosition(monster_state.pos);
			m_Monsters[monster_id]->setPosition(maths::vec2{ monster_state.pos.x ,m_Monsters[monster_id]->getPosition().y });
			m_Monsters[monster_id]->setControls(0, monster_state.dir == network::Left);
			m_Monsters[monster_id]->setControls(1, monster_state.dir == network::Right);
		}

		void MonsterManager::killMonster(unsigned int monster_id)
		{
			for (auto monster = m_Layer.m_Renderables.begin(); monster != m_Layer.m_Renderables.end();)
			{
				if ((*monster)->getId() == monster_id)
				{
					delete (*monster);
					monster = m_Layer.m_Renderables.erase(monster);
					m_Monsters.erase(monster_id);
					EventManager* m_EventManager = EventManager::Instance();
					m_EventManager->execute<unsigned int>(MonsterDied, monster_id);
				}
				else
				{
					++monster;
				}
			}
		}

		//void MonsterManager::checkCollision()
		//{
		//	if (!m_Char->is_hit)
		//		for (auto monster : m_Layer.m_Renderables)
		//		{
		//			if (monster->check_collision(m_Char->getHitBox()))
		//			{
		//				std::cout << "colliding" << std::endl;
		//				if (monster->getBounds().GetBottomMiddle().x < m_Char->getBounds().GetBottomMiddle().x)
		//					m_Char->getHit(game::Direction::Right, monster->getDamage());
		//				else
		//					m_Char->getHit(game::Direction::Left, monster->getDamage());
		//				return;
		//			}
		//		}
		//}

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

		void MonsterManager::clearMonsters()
		{
			m_Layer.clear();
			m_Monsters.clear();
		}
	}
}
