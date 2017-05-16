#include "monster_manager.h"

namespace hiraeth {
	namespace game {

		MonsterManager::MonsterManager(map::MapLayer* map_layer, game::Character* character)
			: m_MapLayer(map_layer), 
			m_Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), 
			m_Layer(&m_Shader),
			m_Char(character)
		{
			//m_Monsters.push_back(new Monster(maths::vec2(-200, 0), m_MapLayer));
			//m_Monsters.push_back(new Monster(maths::vec2(200, 0), m_MapLayer));
			m_Layer.add(new Monster(maths::vec2(-200, 0), m_MapLayer));
			m_Layer.add(new Monster(maths::vec2(200, 0), m_MapLayer));
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

	}
}