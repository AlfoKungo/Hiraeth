#include "monster_manager.h"

namespace hiraeth {
	namespace game {

		MonsterManager::MonsterManager(map::MapLayer* map_layer, game::Character* character)
			: m_MapLayer(map_layer), 
			m_Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), 
			m_Layer(&m_Shader),
			m_Char(character)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &MonsterManager::mapChanged);
			serialize_data();
			mapChanged();
			m_Char->setMonsters(&m_Layer.m_Renderables);
		}

		void MonsterManager::draw()
		{
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", view::Camera::get_ortho());
			m_Layer.render();
		}

		void MonsterManager::update()
		{
			m_Layer.update();

			//destroying dead monsters
			for (auto monster = m_Layer.m_Renderables.begin(); monster != m_Layer.m_Renderables.end();)
			{
				if ((*monster)->died)
				{
					maths::vec2 starting_position((*monster)->m_StartingPosition);
					unsigned int type = (*monster)->getType();
					delete (*monster);
					monster = m_Layer.m_Renderables.erase(monster);
					m_SummonQueue.push(Summoner{map::Summon{type, starting_position} , StaticTimer::timer.elapsed() + 1.0f});
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
					m_Layer.add(new Monster(deserialize_monster_data(m_SummonQueue.front().summon.monsterType), m_SummonQueue.front().summon.position, m_SummonQueue.front().summon.monsterType, m_MapLayer));
					m_SummonQueue.pop();
				}
				else
					break;
			}

			checkCollision();
		}

		bool MonsterManager::checkCollision()
		{
			if (!m_Char->is_hit)
				for (auto monster : m_Layer.m_Renderables)
				{
					if (monster->checkCollision(m_Char->getBounds()))
					{
						std::cout << "colliding" << std::endl;
						if (monster->getBounds().GetBottomMiddle().x < m_Char->getBounds().GetBottomMiddle().x)
							m_Char->getHit(game::Direction::Left, monster->getDamage());
						else
							m_Char->getHit(game::Direction::Right, monster->getDamage());
						return true;
					}
				}

			return false;
		}

		void MonsterManager::serialize_data()
		{

			std::ofstream file("monster.data");
			cereal::BinaryOutputArchive oarchive(file);
			//oarchive(int(2));
			file.seekp(sizeof(int) * 2);
			int map_data1_location = file.tellp();
			oarchive(MonsterData{ "slime", MonsterFramesAmount{3, 7, 1},
				MonsterStatsStruct{ "Slime", 5, 4444, 400, 400, 250, 250, 15, 40, 40, 40, 30, 30, 20, 20, 100, 100}});
			int map_data2_location = file.tellp();
			oarchive(MonsterData{ "green_mushroom", MonsterFramesAmount{3, 4, 1},
				MonsterStatsStruct{ "Green Mushroom", 5, 4444, 1000, 1000, 250, 250, 15, 40, 40, 40, 30, 30, 20, 20, 100, 100}});
			file.seekp(0);
			oarchive(map_data1_location);
			oarchive(map_data2_location);
		}

		MonsterData MonsterManager::deserialize_monster_data(unsigned int map_index)
		{
			std::ifstream file("monster.data");
			cereal::BinaryInputArchive iarchive(file);
			int start_of_data;
			file.seekg(sizeof(int) * (map_index - 1));
			iarchive(start_of_data);
			file.seekg(start_of_data);
			MonsterData monster_data;
			iarchive(monster_data);
			return monster_data;
		}

		void MonsterManager::mapChanged()
		{
			//clear summon queue
			while (!m_SummonQueue.empty())
				m_SummonQueue.pop();
			m_Layer.clear();
			//reload new summons
			for (auto summon : (m_MapLayer->getSummons()))
				m_SummonQueue.push(Summoner{ summon, StaticTimer::timer.elapsed() + 2.0f});
		}
	}
}
