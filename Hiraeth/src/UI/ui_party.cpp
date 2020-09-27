#include "ui_party.h"

namespace hiraeth {
	namespace ui {
		UiParty::UiParty(maths::vec2 pos)
			: UiWindow(maths::Rectangle{ pos, {640, 470} }, party),
			m_PartyMemebersLabel{ "arial", 26, "nigges", {20,400}, 0xff000000, graphics::Label::Alignment::LEFT }
		{

			m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/Party/Party.list.backgrnd.png")));

			m_ForegroundGroup->add_ref(&m_PartyMemebersLabel);
			m_PartyMemebersLabel.setText("no niggas");
		}
	}
}
