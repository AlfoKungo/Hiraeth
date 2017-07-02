#include "item_manager.h"

namespace hiraeth {
	namespace item {

		ItemManager::ItemManager(const std::vector<physics::FootHold>& foot_holds)
			: 
			m_Items(new graphics::Shader("src/shaders/basic.vert", "src/shaders/basic.frag"), true),
			m_FootHolds(foot_holds)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(MapChanged, this, &ItemManager::mapChanged);
			m_Items.add(new Item(maths::vec2(0), graphics::TextureManager::Load("Assets/items/wing.icon.png"), foot_holds));
			m_Items.add(new Item(maths::vec2(-200, 0), graphics::TextureManager::Load("Assets/items/dagger.icon.png"), foot_holds));
			m_Items.add(new Item(maths::vec2(200, 0), graphics::TextureManager::Load("Assets/items/staff.icon.png"), foot_holds));

		}

		void ItemManager::draw() const
		{
			m_Items.render();
		}

		void ItemManager::update()
		{
			m_Items.update();
		}

		void ItemManager::dropItem(const char* name, maths::vec2 pos)
		{
			m_Items.add(new Item(pos, graphics::TextureManager::Load("Assets/items/wing.icon.png"), m_FootHolds));
		}

		void ItemManager::mapChanged()
		{
			m_Items.clear();
		}
	}
}