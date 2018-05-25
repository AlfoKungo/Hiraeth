#include "ui_equip.h"

namespace hiraeth {
	namespace ui {

		UiEquip::UiEquip(maths::vec2 pos, input::Controls control_key)
			: UiWindow(maths::Rectangle(pos.x, pos.y, 184, 290), control_key)
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(StatsUpdate, this, &UiEquip::StatsUpdated);
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 5), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd2.png")));
			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(10, 32), graphics::TextureManager::Load("Assets/UI/Equip/Equip.character.backgrnd3.png")));

			fillGroup();
		}

		void UiEquip::fillGroup()
		{
			m_ForegroundGroup->clear();
		}

		void UiEquip::StatsUpdated()
		{
			fillGroup();
		}

		void UiEquip::mouse_clicked(maths::vec2 mousePos)
		{
		}

	}
}
